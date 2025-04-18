use super::{statement::*, token::WonTokenKind};
use crate::{
  parser::{Parser, ParserError},
  token::{Span, SpanContainer, Token},
};

use std::{iter::Peekable, vec::IntoIter};

pub struct WonParser;
impl<'src> Parser<'src, WonTokenKind, Statement<'src>> for WonParser {
  fn parse(tokens: Vec<Token<'src, WonTokenKind>>) -> Vec<Statement<'src>> {
    let mut token_iterator = tokens.into_iter().peekable();
    let mut output = Vec::new();

    while token_iterator.peek().is_some() {
      let kind = token_iterator.peek().unwrap().kind;
      output.push(
        match kind {
          WonTokenKind::Let => {
            let token = token_iterator.next().unwrap();
            Self::parse_decl_assign(
              &mut token_iterator,
              Some(SpanContainer::from_token(token, StorageKind::Simple)),
            )
          }
          WonTokenKind::Const => {
            let token = token_iterator.next().unwrap();
            Self::parse_decl_assign(
              &mut token_iterator,
              Some(SpanContainer::from_token(token, StorageKind::Constant)),
            )
          }
          WonTokenKind::EndStatement => {
            token_iterator.next();
            Ok(Statement::Trail)
          }
          _ => {
            Self::parse_expression(&mut token_iterator, ExpressionContext::Simple).map(|e| e.into())
          }
        }
        .unwrap(),
      );
    }

    output
  }
}

type StatementResult<'src> = Result<Statement<'src>, ParserError<WonTokenKind>>;
type ExpressionResult<'src> =
  Result<SpanContainer<'src, Expression<'src>>, ParserError<WonTokenKind>>;
type TokenResult<'src> = Result<Token<'src, WonTokenKind>, ParserError<WonTokenKind>>;

impl WonParser {
  fn parse_decl_assign<'src>(
    tokens: &mut Peekable<IntoIter<Token<'src, WonTokenKind>>>,
    storage: Option<SpanContainer<'src, StorageKind>>,
  ) -> StatementResult<'src> {
    let identifier = Self::expect_token(tokens.next(), WonTokenKind::Identifier)?.span;

    match tokens.next().ok_or(ParserError::UnexpectedEOF)?.kind {
      WonTokenKind::DeclType => {
        let ty = Some(Self::expect_token(tokens.next(), WonTokenKind::Identifier)?.span);
        let mut expression = None;

        match tokens.next().ok_or(ParserError::UnexpectedEOF)?.kind {
          WonTokenKind::Assign => {
            expression = Some(Self::parse_expression(tokens, ExpressionContext::Simple)?);
          }
          WonTokenKind::EndStatement => {}
          kind => {
            return Err(ParserError::UnexpectedToken(
              kind,
              WonTokenKind::EndStatement,
            ));
          }
        }

        Ok(Statement::Assignement {
          storage,
          identifier,
          ty,
          expression,
        })
      }
      WonTokenKind::Assign => Ok(Statement::Assignement {
        storage,
        identifier,
        ty: None,
        expression: Some(Self::parse_expression(tokens, ExpressionContext::Simple)?),
      }),
      WonTokenKind::EndStatement => Ok(Statement::Assignement {
        storage,
        identifier,
        ty: None,
        expression: None,
      }),
      kind => Err(ParserError::UnexpectedToken(
        kind,
        WonTokenKind::EndStatement,
      )),
    }
  }

  fn parse_expression<'src>(
    tokens: &mut Peekable<IntoIter<Token<'src, WonTokenKind>>>,
    context: ExpressionContext,
  ) -> ExpressionResult<'src> {
    let lhs_token = tokens.next().unwrap();
    let lhs_kind = lhs_token.kind;
    let lhs_span = lhs_token.span;
    let lhs = match lhs_kind {
      WonTokenKind::GroupStart => Self::parse_expression(tokens, ExpressionContext::InnerGroup),
      WonTokenKind::True | WonTokenKind::False => Ok(SpanContainer::from_token(
        lhs_token,
        Expression::Literal(LiteralKind::Bool),
      )),
      WonTokenKind::Number => Ok(SpanContainer::from_token(
        lhs_token,
        Expression::Literal(LiteralKind::Integer),
      )),
      WonTokenKind::Identifier => Ok(SpanContainer::from_token(
        lhs_token,
        Expression::Access(Access),
      )),
      kind => Err(ParserError::ExpectedStatement(kind)),
    }?;

    // Need to check for operators real soon!
    let rhs_token = tokens.peek().unwrap();
    let rhs_kind = rhs_token.kind;
    match rhs_kind {
      WonTokenKind::As => {
        tokens.next();
        let rhs_span = Self::expect_token(tokens.next(), WonTokenKind::Identifier)?.span;

        Self::expect_token(
          tokens.next(),
          if context == ExpressionContext::InnerGroup {
            WonTokenKind::GroupEnd
          } else {
            WonTokenKind::EndStatement
          },
        )?;

        Ok(SpanContainer {
          span: Span::from(&lhs_span, &rhs_span),
          item: Expression::PrimitiveCast {
            expression: lhs.boxed(),
            ty: rhs_span,
          },
        })
      }
      WonTokenKind::GroupStart => {
        tokens.next();
        if lhs_kind == WonTokenKind::Identifier {
          let mut items = Vec::new();

          while tokens.peek().is_some() {
            let token = tokens.peek().unwrap();
            let kind = token.kind;

            items.push(match kind {
              WonTokenKind::GroupEnd => break,
              WonTokenKind::ItemSeparator => {
                tokens.next();
                continue;
              }
              _ => Self::parse_expression(tokens, ExpressionContext::Pack),
            }?);
          }

          let end = Self::expect_token(tokens.next(), WonTokenKind::GroupEnd)?;
          Self::expect_token(tokens.next(), WonTokenKind::EndStatement)?;
          Ok(SpanContainer {
            span: Span::from(&lhs_span, &end.span),
            item: Expression::FunctionCall {
              access: lhs.map(|e| e.into()),
              items,
            },
          })
        } else {
          Err(ParserError::UnexpectedToken(
            rhs_kind,
            if context == ExpressionContext::InnerGroup {
              WonTokenKind::GroupEnd
            } else {
              WonTokenKind::EndStatement
            },
          ))
        }
      }
      kind => {
        if context.should_break(&kind) {
          Ok(lhs)
        } else {
          Err(match kind {
            WonTokenKind::ItemSeparator | WonTokenKind::GroupEnd | WonTokenKind::EndStatement => {
              ParserError::UnexpectedToken(
                kind,
                if context == ExpressionContext::InnerGroup {
                  WonTokenKind::GroupEnd
                } else {
                  WonTokenKind::EndStatement
                },
              )
            }
            kind => ParserError::ExpectedStatement(kind),
          })
        }
      }
    }
  }

  fn expect_token(next: Option<Token<'_, WonTokenKind>>, kind: WonTokenKind) -> TokenResult<'_> {
    let token = next.ok_or(ParserError::UnexpectedEOF)?;

    if token.kind == kind {
      Ok(token)
    } else {
      Err(ParserError::UnexpectedToken(token.kind, kind))
    }
  }
}
