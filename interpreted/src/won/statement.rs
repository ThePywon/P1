use crate::token::{Span, SpanContainer};


#[derive(Debug)]
pub enum Statement<'src> {
  Assignement {
    storage: Option<SpanContainer<'src, StorageKind>>,
    identifier: Span<'src>,
    ty: Option<SpanContainer<'src, Type>>,
    expression: Option<SpanContainer<'src, Expression>>
  }
}

#[derive(Debug)]
pub enum StorageKind {
  Simple,
  Constant
}

#[derive(Debug)]
pub enum Type {
  Bool
}

#[derive(Debug)]
pub enum Expression {
  Literal(LiteralExprKind)
}

#[derive(Debug)]
pub enum LiteralExprKind {
  Bool,
  Integer
}
