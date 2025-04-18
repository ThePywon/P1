use crate::token::{Span, SpanContainer};

use super::token::WonTokenKind;

#[derive(Debug)]
pub enum Statement<'src> {
  Assignement {
    storage: Option<SpanContainer<'src, StorageKind>>,
    identifier: Span<'src>,
    ty: Option<Span<'src>>,
    expression: Option<SpanContainer<'src, Expression<'src>>>,
  },
  Trail,
  Expression(SpanContainer<'src, Expression<'src>>),
}
impl<'src> From<SpanContainer<'src, Expression<'src>>> for Statement<'src> {
  fn from(value: SpanContainer<'src, Expression<'src>>) -> Self {
    Self::Expression(value)
  }
}

#[derive(Debug)]
pub enum Expression<'src> {
  FunctionCall {
    access: SpanContainer<'src, Access>,
    items: Vec<SpanContainer<'src, Expression<'src>>>,
  },
  Access(Access),
  Literal(LiteralKind),
  PrimitiveCast {
    expression: SpanContainer<'src, Box<Expression<'src>>>,
    ty: Span<'src>,
  },
}

#[derive(Debug, PartialEq, Eq)]
pub enum ExpressionContext {
  Simple,
  InnerGroup,
  Pack,
}
impl ExpressionContext {
  pub fn should_break(&self, kind: &WonTokenKind) -> bool {
    match kind {
      WonTokenKind::EndStatement => *self == Self::Simple,
      WonTokenKind::GroupEnd => *self == Self::InnerGroup || *self == Self::Pack,
      WonTokenKind::ItemSeparator => *self == Self::Pack,
      _ => false,
    }
  }
}

#[derive(Debug)]
pub struct Access;
impl<'src> From<Expression<'src>> for Access {
  fn from(value: Expression<'src>) -> Self {
    if let Expression::Access(access) = value {
      access
    } else {
      Access
    }
  }
}

#[derive(Debug)]
pub enum StorageKind {
  Simple,
  Constant,
}

#[derive(Debug)]
pub enum LiteralKind {
  Bool,
  Integer,
}
