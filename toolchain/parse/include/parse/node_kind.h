#ifndef VIPER_TOOLCHAIN_PARSE_NODE_KIND_H
#define VIPER_TOOLCHAIN_PARSE_NODE_KIND_H

namespace viper::toolchain::parse
{
	enum NodeKind
	{
		FileStart,

		FunctionIntroducer, // "define"
		FunctionDefinition, // "define" <identifier> "(" <parameter list> ")" <block>
		ParamterList,       // <parameter list> ::= <parameter> ("," <parameter>)*
		CodeBlock,          // "{" <statement>* "}"
	};
} // namespace viper::toolchain::parse

#endif // VIPER_TOOLCHAIN_PARSE_NODE_KIND_H
