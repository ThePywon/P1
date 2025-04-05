use super::token::{Token, TokenKind};

pub trait Lexer<K: TokenKind> {
  fn lex<'input>(input: &'input [u8]) -> Vec<Token<'input, K>>;
}
