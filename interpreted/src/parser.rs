use super::token::{Token, TokenKind};

use thiserror::Error;

pub trait Parser<'src, K: TokenKind, Output> {
  fn parse(tokens: Vec<Token<'src, K>>) -> Vec<Output>;
}

#[derive(Error, Debug)]
pub enum ParserError<'src, K: TokenKind> {
  #[error("Unexpected end of file.")]
  UnexpectedEOF,
  #[error("Unexpected token, got {0}, expected {1}")]
  UnexpectedToken(K, K),
  #[error("Type for name \"{0}\" not found.")]
  TypeNotFound(&'src str),
  #[error("Expected statement, got {0}")]
  ExpectedStatement(K)
}
