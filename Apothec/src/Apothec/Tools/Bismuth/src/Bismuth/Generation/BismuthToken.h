#pragma once

#include <optional>
#include <string>

#include "../Data/types.h"

namespace bismuth::generation
{

	enum class TokenType
	{
		ProjectTag,               // project
		ProjectLink,              // add
					              
		FunctionTag,              // function
		ComponentTag,             // component
		ArchetypeTag,             // arch
		InterfaceTag,             // interface
		SystemTag,                // system
		EntityTag,                // entity
						          
		ComponentInclusion,       // with
		ComponentRequirement,     // requires
		ArchetypeInclusion,       // extends
		SystemAccess,             // using
		InterfaceImplementation,  // impl
		InterfaceLink,            // from
		EntityConstruction,       // construct

		InterfaceAttempt,         // try
		InterfaceCatch,           // catch

		FunctionReturn,           // return

		ScopeStart,               // {
		ScopeEnd,                 // }
		ExpressionStart,          // ( 
		ExpressionEnd,            // )

		LineEnd,                  // ;
		ReturnHint,               // ->
		Assignment,               // =
		Property,                 // :

		ImmediateInt32,           // xxx 
		ImmediateUint32,          // xxx_u
		ImmediateInt64,           // xxx_l
		ImmediateUint64,          // xxx_lu 0r ul
		ImmediateFloat,           // xxx.xxx
		ImmediateDouble,          // xxx.xxx_l
		ImmediateBoolean,         // true or false
		ImmediateString,          // "xxx"

		Identifier,               // other
	};

	struct Token
	{
		TokenType Type;
		std::optional<PropertyID> Value;

		Token(TokenType type, PropertyID val)
			: Type(type)
			, Value(val)
		{}

		Token(TokenType type)
			: Type(type)
			, Value(std::nullopt)
		{}
	};

}