
struct HiseJavascriptEngine::RootObject::RegisterVarStatement : public Statement
{
	RegisterVarStatement(const CodeLocation& l) noexcept : Statement(l) {}

	ResultCode perform(const Scope& s, var*) const override
	{
		varRegister->addRegister(name, initialiser->getResult(s));
		return ok;
	}

	VarRegister* varRegister = nullptr;

	Identifier name;
	ExpPtr initialiser;
};


struct HiseJavascriptEngine::RootObject::RegisterAssignment : public Expression
{
	RegisterAssignment(const CodeLocation &l, int registerId, ExpPtr source_) noexcept: Expression(l), registerIndex(registerId), source(source_) {}

	var getResult(const Scope &s) const override
	{
		var value(source->getResult(s));

		VarRegister* reg = &s.root->hiseSpecialData.varRegister;
		reg->setRegister(registerIndex, value);
		return value;
	}

	int registerIndex;

	ExpPtr source;
};

struct HiseJavascriptEngine::RootObject::RegisterName : public Expression
{
	RegisterName(const CodeLocation& l, const Identifier& n, VarRegister* rootRegister_, int indexInRegister_, var* data_) noexcept : 
	  Expression(l), 
	  rootRegister(rootRegister_),
	  indexInRegister(indexInRegister_),
      name(n), 
	  data(data_) {}

	var getResult(const Scope& /*s*/) const override
	{
		return *data;
	}

	void assign(const Scope& /*s*/, const var& newValue) const override
	{
		*data = newValue;
	}

	VarRegister* rootRegister;
	int indexInRegister;

	var* data;

	Identifier name;
};


struct HiseJavascriptEngine::RootObject::ApiConstant : public Expression
{
	ApiConstant(const CodeLocation& l) noexcept : Expression(l) {}
	var getResult(const Scope&) const override   { return value; }

	var value;
};

struct HiseJavascriptEngine::RootObject::ApiCall : public Expression
{
	ApiCall(const CodeLocation &l, ApiClass *apiClass_, int expectedArguments_, int functionIndex) noexcept:
	Expression(l),
		expectedNumArguments(expectedArguments_),
		functionIndex(functionIndex),
		apiClass(apiClass_)
	{
		for (int i = 0; i < 5; i++)
		{
			argumentList[i] = nullptr;
		}
	};

	var getResult(const Scope& s) const override
	{
		var results[5];
		for (int i = 0; i < expectedNumArguments; i++)
		{
			results[i] = argumentList[i]->getResult(s);
		}

		CHECK_CONDITION_WITH_LOCATION(apiClass != nullptr, "API class does not exist");

		return apiClass->callFunction(functionIndex, results, expectedNumArguments);
	}

	const int expectedNumArguments;

	ExpPtr argumentList[5];
	const int functionIndex;
	

	const ReferenceCountedObjectPtr<ApiClass> apiClass;
};


struct HiseJavascriptEngine::RootObject::ConstObjectApiCall : public Expression
{
	ConstObjectApiCall(const CodeLocation &l, var *objectPointer_, const Identifier& functionName_) noexcept:
	Expression(l),
		objectPointer(objectPointer_),
		functionName(functionName_),
		expectedNumArguments(-1),
		functionIndex(-1),
		initialised(false)
	{
		for (int i = 0; i < 4; i++)
		{
			argumentList[i] = nullptr;
		}
	};

	var getResult(const Scope& s) const override
	{
		if (!initialised)
		{
			initialised = true;

			CHECK_CONDITION_WITH_LOCATION(objectPointer != nullptr, "Object Pointer does not exist");

			object = dynamic_cast<ConstScriptingObject*>(objectPointer->getObject());

			CHECK_CONDITION_WITH_LOCATION(object != nullptr, "Object doesn't exist");

			object->getIndexAndNumArgsForFunction(functionName, functionIndex, expectedNumArguments);

			CHECK_CONDITION_WITH_LOCATION(functionIndex != -1, "function " + functionName.toString() + " not found.");
		}

		var results[5];

		for (int i = 0; i < expectedNumArguments; i++)
		{
			results[i] = argumentList[i]->getResult(s);
		}

		CHECK_CONDITION_WITH_LOCATION(object != nullptr, "Object does not exist");

		return object->callFunction(functionIndex, results, expectedNumArguments);
	}

	
	mutable bool initialised;
	ExpPtr argumentList[4];
	mutable int expectedNumArguments;
	mutable int functionIndex;
	Identifier functionName;

	var* objectPointer;

	mutable ReferenceCountedObjectPtr<ConstScriptingObject> object;
};

struct HiseJavascriptEngine::RootObject::InlineFunction
{
	struct FunctionCall;

	struct Object : public DynamicObject,
					public DebugableObject
	{
	public:

		Object(Identifier &n, const Array<Identifier> &p) : 
			name(n),
			e(nullptr)
		{
			parameterNames.addArray(p);

			functionDef = name.toString();
			functionDef << "(";

			for (int i = 0; i < parameterNames.size(); i++)
			{
				functionDef << parameterNames[i].toString();
				if (i != parameterNames.size()-1) functionDef << ", ";
			}

			functionDef << ")";
		}

		~Object()
		{
			parameterNames.clear();
			body = nullptr;
		}

		String getDebugValue() const override { return lastReturnValue.toString(); }

		/** This will be shown as name of the object. */
		String getDebugName() const override { return functionDef; }

		String getDebugDataType() const override { return DebugInformation::getVarType(lastReturnValue); }

		void doubleClickCallback(const MouseEvent &/*event*/, Component* ed)
		{
			DebugableObject::Helpers::gotoLocation(ed, location);
		}

		AttributedString getDescription() const override 
		{ 
			return DebugableObject::Helpers::getFunctionDoc(commentDoc, parameterNames); 
		}

		void setFunctionCall(const FunctionCall *e_)
		{
			e = e_;
		}

		Identifier name;
		Array<Identifier> parameterNames;
		typedef ReferenceCountedObjectPtr<Object> Ptr;
		ScopedPointer<BlockStatement> body;

		String functionDef;
		String commentDoc;

		var lastReturnValue = var::undefined();
		
		const FunctionCall *e;

		NamedValueSet localProperties;

		Location location;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Object)

	};

	struct FunctionCall : public Expression
	{
		FunctionCall(const CodeLocation &l, Object *referredFunction) : 
			Expression(l),
			f(referredFunction),
			numArgs(f->parameterNames.size())
		{
			for (int i = 0; i < numArgs; i++)
			{
				parameterResults.add(var::undefined());
			}
		};

		~FunctionCall()
		{
			f = nullptr;
		}

		void addParameter(Expression *e)
		{
			parameterExpressions.add(e);
		}

		var getResult(const Scope& s) const override
		{
			f->setFunctionCall(this);

			for (int i = 0; i < numArgs; i++)
			{
				parameterResults.setUnchecked(i, parameterExpressions.getUnchecked(i)->getResult(s));
			}

			ResultCode c = f->body->perform(s, &returnVar);

			for (int i = 0; i < numArgs; i++)
			{
				parameterResults.setUnchecked(i, var::undefined());
			}

			f->lastReturnValue = returnVar;

			f->setFunctionCall(nullptr);

			if (c == Statement::returnWasHit) return returnVar;
			else return var::undefined();
		}

		Object::Ptr f;

		OwnedArray<Expression> parameterExpressions;
		mutable Array<var> parameterResults;

		mutable var returnVar;

		const int numArgs;
	};

	struct ParameterReference : public Expression
	{
		ParameterReference(const CodeLocation &l, Object *referedFunction, int id):
			Expression(l),
			index(id),
			f(referedFunction)
		{}

		~ParameterReference()
		{
			f = nullptr;
		}

		var getResult(const Scope&) const override 
		{
			if (f->e != nullptr)
			{
				return  (f->e->parameterResults[index]);
			}
			else
			{
				location.throwError("Accessing parameter reference outside the function call");
				return var();
			}
		}

		Object* f;
		int index;
	};
};



struct HiseJavascriptEngine::RootObject::GlobalVarStatement : public Statement
{
	GlobalVarStatement(const CodeLocation& l) noexcept : Statement(l) {}

	ResultCode perform(const Scope& s, var*) const override
	{
		s.root->hiseSpecialData.globals->setProperty(name, initialiser->getResult(s));
		return ok;
	}

	Identifier name;
	ExpPtr initialiser;
};



struct HiseJavascriptEngine::RootObject::GlobalReference : public Expression
{
	GlobalReference(const CodeLocation& l, DynamicObject *globals_, const Identifier &id_) noexcept : Expression(l), globals(globals_), id(id_) {}

	var getResult(const Scope& s) const override
	{
		return s.root->hiseSpecialData.globals->getProperty(id);
	}

	void assign(const Scope& s, const var& newValue) const override
	{
		s.root->hiseSpecialData.globals->setProperty(id, newValue);
	}

	DynamicObject::Ptr globals;
	const Identifier id;

	int index;
};



struct HiseJavascriptEngine::RootObject::LocalVarStatement : public Statement
{
	LocalVarStatement(const CodeLocation& l, InlineFunction::Object* parentFunction_) noexcept : Statement(l), parentFunction(parentFunction_) {}

	ResultCode perform(const Scope& s, var*) const override
	{
		parentFunction->localProperties.set(name, initialiser->getResult(s));
		return ok;
	}

	mutable InlineFunction::Object* parentFunction;
	Identifier name;
	ExpPtr initialiser;
};



struct HiseJavascriptEngine::RootObject::LocalReference : public Expression
{
	LocalReference(const CodeLocation& l, InlineFunction::Object *parentFunction_, const Identifier &id_) noexcept : Expression(l), parentFunction(parentFunction_), id(id_) {}

	var getResult(const Scope& /*s*/) const override
	{
		return parentFunction->localProperties[id];
	}

	void assign(const Scope& /*s*/, const var& newValue) const override
	{
		parentFunction->localProperties.set(id, newValue);
	}

	InlineFunction::Object* parentFunction;
	const Identifier id;

	int index;
};



struct HiseJavascriptEngine::RootObject::CallbackParameterReference: public Expression
{
	CallbackParameterReference(const CodeLocation& l, var* data_) noexcept : Expression(l), data(data_) {}

	var getResult(const Scope& /*s*/) const override
	{
		return *data;
	}

	var* data;
};

struct HiseJavascriptEngine::RootObject::CallbackLocalStatement : public Statement
{
	CallbackLocalStatement(const CodeLocation& l, Callback* parentCallback_) noexcept : Statement(l), parentCallback(parentCallback_) {}

	ResultCode perform(const Scope& s, var*) const override
	{
		parentCallback->localProperties.set(name, initialiser->getResult(s));
		return ok;
	}

	mutable Callback* parentCallback;
	Identifier name;
	ExpPtr initialiser;
};

struct HiseJavascriptEngine::RootObject::CallbackLocalReference : public Expression
{
	CallbackLocalReference(const CodeLocation& l, var* data_) noexcept : Expression(l), data(data_) {}

	var getResult(const Scope& /*s*/) const override
	{
		return *data;
	}

	var* data;
};

struct HiseJavascriptEngine::RootObject::ExternalCFunction: public ReferenceCountedObject,
														    public DebugableObject
{
	ExternalCFunction(CodeLocation& l, const Identifier &name_, bool hasReturnType_, Array<Identifier>& arguments_, const String &comment_, const String& codeToCompile):
	name(name_),
	hasReturnType(hasReturnType_),
	numArguments(arguments_.size()),
	arguments(arguments_),
	f(nullptr),
	compiledOk(false),
	commentDoc(comment_),
	signature(codeToCompile.fromFirstOccurrenceOf(" ", false, false).upToFirstOccurrenceOf(")", true, false))
	{
		String cCode = "#include <TccLibrary.h>\n";
		cCode << "#include <math.h>\n";
		cCode << codeToCompile;

#if JUCE_IOS
#else
		int dllLoading = c.openContext();

		if (dllLoading != (int)LoadingErrorCode::LoadingSuccessful)
		{
			c.closeContext();
			l.throwError("The TCC compiler dynamic library could not be loaded.");
		}

		compiledOk = c.compile(cCode) == 0;

		if (compiledOk)
		{
			f = c.getFunction(name.toString());
			c.closeContext();
			if (f == nullptr)
			{
				l.throwError("The function couldn't be parsed");
			}
		}
		else
		{
			c.closeContext();
			l.throwError("Error at compiling external C function " + name.toString());
		}
#endif
	}

	struct FunctionCall : public Expression
	{
		FunctionCall(const CodeLocation& l, ExternalCFunction* cFunction_) : Expression(l), cFunction(cFunction_) {}

		var getResult(const Scope& s) const override
		{
			if (!cFunction->compiledOk)
			{
				location.throwError("Trying to call a uncompiled function.");
				return var();
			}

			var args[4];

			for (int i = 0; i < cFunction->numArguments; i++)
			{
				args[i] = parameterExpressions[i]->getResult(s);
			}

			if (cFunction->hasReturnType)
			{
				
			}
			else
			{
				using voidFunc0 = void(*)();
				using voidFunc1 = void(*)(var*);
				using voidFunc2 = void(*)(var*, var*);
				using voidFunc3 = void(*)(var*, var*, var*);
				using voidFunc4 = void(*)(var*, var*, var*, var*);

				switch (cFunction->numArguments)
				{
				case 0: ((voidFunc0)cFunction->f)(); break;
				case 1: ((voidFunc1)cFunction->f)(args); break;
				case 2: ((voidFunc2)cFunction->f)(args, args + 1); break;
				case 3: ((voidFunc3)cFunction->f)(args, args + 1, args + 2); break;
				case 4: ((voidFunc4)cFunction->f)(args, args + 1, args + 2, args + 3); break;
				}
			}
			
			return var();
		}

		OwnedArray<Expression> parameterExpressions;
		ExternalCFunction* cFunction;
	};


	String getDebugValue() const override { return lastReturnValue.toString(); }

	/** This will be shown as name of the object. */
	String getDebugName() const override { return signature; }

	String getDebugDataType() const override { return DebugInformation::getVarType(lastReturnValue); }

	AttributedString getDescription() const override
	{
		return DebugableObject::Helpers::getFunctionDoc(commentDoc, arguments);
	}

	var lastReturnValue;

	String signature;

	Identifier name;
	bool hasReturnType;
	Array<Identifier> arguments;
	int numArguments;
	void* f;
    
#if JUCE_IOS
#else
	TccContext c;
#endif
	bool compiledOk;
	String commentDoc;
};