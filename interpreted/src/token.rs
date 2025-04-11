use std::{fmt::Debug, str};

#[derive(Clone, Copy)]
pub struct Span<'src> {
  pub index: usize,
  pub length: usize,
  pub source: &'src [u8]
}
impl<'src> Span<'src> {
  pub fn get_content(&self) -> &'src str {
    &str::from_utf8(&self.source[self.index..(self.index+self.length)]).unwrap()
  }

  pub fn end(&self) -> usize { self.index + self.length }
}
impl<'src> Debug for Span<'src> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    f.debug_struct("Span")
      .field("index", &self.index)
      .field("length", &self.length)
      .field("value", &self.get_content())
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
  pub span: Span<'src>,
  pub item: T
}
impl<'src, T> SpanContainer<'src, T> {
  pub fn from_token<K: TokenKind>(token: Token<'src, K>, item: T) -> Self {
    Self { span: token.span, item }
  }

  pub fn boxed(self) -> SpanContainer<'src, Box<T>> {
    SpanContainer { span: self.span, item: Box::new(self.item) }
  }

  pub fn map<O>(self, f: fn(T) -> O) -> SpanContainer<'src, O> {
    SpanContainer { span: self.span, item: f(self.item) }
  }
}
