#pragma once

#include "BismuthToken.h"
#include "../Data/types.h"

#include <vector>
#include <map>

namespace bismuth::generation
{

	class Tokenizer
	{
	public:
		explicit Tokenizer(const std::string& filePath);

		inline const std::vector<Token> GetTokens() const { return m_Tokens; }

		static inline std::hash<std::string> GetHasher() { return s_Hasher; }

	private:
		inline void PushToken(TokenType type, const std::string& val)
		{
			m_Tokens.push_back(Token(type, s_Hasher(val)));
			m_ImmediateProperties[s_Hasher(val)] = val;
		}

		inline void PushToken(TokenType type)
		{
			m_Tokens.push_back(Token(type));
		}


		std::vector<Token> m_Tokens;
		std::map<PropertyID, std::string> m_ImmediateProperties;

		static std::hash<std::string> s_Hasher;
	};

}