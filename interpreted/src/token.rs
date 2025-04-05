use std::{fmt::Debug, str};

pub struct Span<'src> {
  pub index: usize,
  pub length: usize,
  pub source: &'src [u8]
}
impl<'src> Span<'src> {
  pub fn get(&self) -> &'src str {
    &str::from_utf8(&self.source[self.index..(self.index+self.length)]).unwrap()
  }
}
impl<'src> Debug for Span<'src> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    f.debug_struct("Span")
      .field("index", &self.index)
      .field("length", &self.length)
      .field("value", &self.get())
      .finish()
  }
}

pub trait TokenKind {
  fn get_meta(&self) -> &str;
}

#[derive(Debug)]
pub struct Token<'src, K: TokenKind> {
  pub kind: K,
  pub span: Span<'src>
}

#[derive(Debug)]
pub struct SpanContainer<'src, T> {
  span: Span<'src>,
  item: T
}
impl<'src, T> SpanContainer<'src, T> {
  pub fn from_token<K: TokenKind>(token: Token<'src, K>, item: T) -> Self {
    Self { span: token.span, item }
  }
}
