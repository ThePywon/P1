use super::token::{Token, TokenKind};

use thiserror::Error;

pub trait Parser<'src, K: TokenKind, Output> {
  fn parse(tokens: Vec<Token<'src, K>>) -> Vec<Output>;
}

#[derive(Error, Debug)]
pub enum ParserError<K: TokenKind> {
  #[error("Unexpected end of file.")]
  UnexpectedEOF,
  #[error("Unexpected token, got {0}, expected {1}")]
  UnexpectedToken(K, K),
  #[error("Expected statement, got {0}")]
  ExpectedStatement(K),
}

// TODO: Make an error class that handles the error message with indexes and context
