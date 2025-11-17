#include "Tokenizer.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace bismuth::generation
{
	std::hash<std::string> Tokenizer::s_Hasher = std::hash<std::string>();

	Tokenizer::Tokenizer(const std::string& filePath)
	{
		std::ifstream file(filePath);
		if (file.is_open())
		{

			std::string c_line;
			bool exclude = false;

			while (std::getline(file, c_line))
			{
				size_t index = 0;
				std::stringstream buffer{};

				while (index < c_line.size())
				{
					char c_char{ c_line[index] };

					if (exclude)
					{
						if (c_char == '-' && c_line[index + 1] == '-')
						{
							exclude = false;
							++index;
						}
						++index;
					}
					else if (isspace(c_char)) // skip white space
						++index;
					else if (isdigit(c_char)) // buffer Immediate numbers
					{
						bool floatingPoint = false;
						bool specifiedType = false;
						bool IsUnsigned = false;
						bool IsExtended = false;

						// buffer until next space
						while (!isspace(c_char) && index < c_line.size())
						{
							if (specifiedType)
							{
								if (c_char == 'u')
									IsUnsigned = true;
								else if (c_char == 'l')
									IsExtended = true;
							}
							else
							{
								if (c_char == '.')
									floatingPoint = true;
								if (c_char == '_')
									specifiedType = true;
								else
									buffer << c_char; // doesn't need to buffer the specifiers
							}

							++index;
							c_char = c_line[index];
						}

						const std::string token_str{ buffer.str() };
						buffer.str(""); // drop buffer

						if (floatingPoint)
							if (IsExtended)
								PushToken(TokenType::ImmediateDouble, token_str);
							else
								PushToken(TokenType::ImmediateFloat, token_str);
						else
							if (IsExtended && IsUnsigned)
								PushToken(TokenType::ImmediateUint64, token_str);
							else if (IsExtended)
								PushToken(TokenType::ImmediateInt64, token_str);
							else if (IsUnsigned)
								PushToken(TokenType::ImmediateUint32, token_str);
							else
								PushToken(TokenType::ImmediateInt32, token_str);

					}
					else if (isalnum(c_char)) // buffer alphanumeric characters
					{
						// buffer to the next non-alnum character
						while (isalnum(c_char) && index < c_line.size())
						{
							buffer << c_char;
							++index;
							c_char = c_line[index];
						}

						const std::string token_str{ buffer.str() };
						buffer.str(""); // drop buffer

						// check types
						if (token_str == "project")
							PushToken(TokenType::ProjectTag);
						else if (token_str == "add")
							PushToken(TokenType::ProjectLink);
						else if (token_str == "component")
							PushToken(TokenType::ComponentTag);
						else if (token_str == "function")
							PushToken(TokenType::FunctionTag);
						else if (token_str == "arch")
							PushToken(TokenType::ArchetypeTag);
						else if (token_str == "with")
							PushToken(TokenType::ComponentInclusion);
						else if (token_str == "interface")
							PushToken(TokenType::InterfaceTag);
						else if (token_str == "requires")
							PushToken(TokenType::ComponentRequirement);
						else if (token_str == "system")
							PushToken(TokenType::SystemTag);
						else if (token_str == "entity")
							PushToken(TokenType::EntityTag);
						else if (token_str == "extends")
							PushToken(TokenType::ArchetypeInclusion);
						else if (token_str == "using")
							PushToken(TokenType::SystemAccess);
						else if (token_str == "impl")
							PushToken(TokenType::InterfaceImplementation);
						else if (token_str == "from")
							PushToken(TokenType::InterfaceLink);
						else if (token_str == "construct")
							PushToken(TokenType::EntityConstruction);
						else if (token_str == "try")
							PushToken(TokenType::InterfaceAttempt);
						else if (token_str == "catch")
							PushToken(TokenType::InterfaceCatch);
						else if (token_str == "return")
							PushToken(TokenType::FunctionReturn);
						else if (token_str == "true")
							PushToken(TokenType::ImmediateBoolean, "true");
						else if (token_str == "false")
							PushToken(TokenType::ImmediateBoolean, "false");
						
						else
							PushToken(TokenType::Identifier, token_str);


					}
					else // buffer non-alphanumeric characters
					{
						
						// check 2 chars
						if (!isalnum(c_line[index + 1]) && index < c_line.size())
						{
							buffer << c_char << c_line[index + 1];
							const std::string c_2char = buffer.str();
							buffer.str(""); // drop buffer

							if (c_2char == "->")
							{
								m_Tokens.push_back(Token(TokenType::ReturnHint));
								index += 2;
								continue;
							}
							else if (c_2char == "--") // comments
							{
								exclude = true;
								index += 2;
								continue; 
							}
						}


						// check single chars
						if (c_char == '{')
							PushToken(TokenType::ScopeStart);
						else if (c_char == '}')
							PushToken(TokenType::ScopeEnd);
						else if (c_char == '(')
							PushToken(TokenType::ExpressionStart);
						else if (c_char == ')')
							PushToken(TokenType::ExpressionEnd);
						else if (c_char == ';')
							PushToken(TokenType::LineEnd);
						else if (c_char == '=')
							PushToken(TokenType::Assignment);
						else if (c_char == ':')
							PushToken(TokenType::Property);
						else if (c_char == '"') // buffer string
						{
							++index;
							c_char = c_line[index];
							while (c_char != '"')
							{
								buffer << c_char;
								++index;
								c_char = c_line[index];
							}

							PushToken(TokenType::ImmediateString, buffer.str());
							buffer.str("");
						}

						++index;
					}

				}

			}

			file.close();
		}
		else
			std::cout << "[BISMUTH] Error opening file: " << filePath << " File not found.\n";
	}

}