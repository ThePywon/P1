use crate::token::TokenKind;
use macros::TokenKind;

#[derive(TokenKind, Clone, Copy, PartialEq, Eq)]
pub enum WonTokenKind {
  #[meta(keyword)] Let,
  #[meta(keyword)] Const,
  #[meta(keyword)] As,
  #[meta(keyword)] False,
  #[meta(keyword)] True,

  #[meta(literal)] Identifier,
  #[meta(literal)] Number,

  #[meta(symbol)] DeclType,
  #[meta(symbol)] EndStatement,

  #[meta(operator)] Assign,
}
