use super::token::{Token, TokenKind};

pub trait Lexer<K: TokenKind> {
  fn lex(input: &[u8]) -> Vec<Token<'_, K>>;
}
