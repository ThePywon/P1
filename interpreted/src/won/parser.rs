use crate::{parser::{Parser, ParserError}, token::{SpanContainer, Token}};
use super::{statement::*, token::WonTokenKind};

use std::{iter::Peekable, vec::IntoIter};

pub struct WonParser;
impl<'src> Parser<'src, WonTokenKind, Statement<'src>> for WonParser {
  fn parse(tokens: Vec<Token<'src, WonTokenKind>>) -> Vec<Statement<'src>> {
    let mut token_iterator = tokens.into_iter().peekable();
    let mut output = Vec::new();

    while token_iterator.peek().is_some() {
      let token = token_iterator.next().unwrap();
      let kind = token.kind;
      output.push(match kind {
        WonTokenKind::Let => Self::parse_assign(&mut token_iterator, Some(SpanContainer::from_token(token, StorageKind::Simple))),
        WonTokenKind::Const => Self::parse_assign(&mut token_iterator, Some(SpanContainer::from_token(token, StorageKind::Constant))),
        _ => continue
      }.unwrap());
    }

    output
  }
}
impl WonParser {
  fn parse_assign<'src>(tokens: &mut Peekable<IntoIter<Token<'src, WonTokenKind>>>, storage: Option<SpanContainer<'src, StorageKind>>) -> Result<Statement<'src>, ParserError<'src, WonTokenKind>> {
    let identifier = Self::expect_token(tokens.next(), WonTokenKind::Identifier)?.span;

    return match tokens.next().ok_or(ParserError::UnexpectedEOF)?.kind {
      WonTokenKind::DeclType => {
        let ty = Some(Self::parse_type(tokens.next())?);
        let mut expression = None;

        match tokens.next().ok_or(ParserError::UnexpectedEOF)?.kind {
          WonTokenKind::Assign => {
            expression = Some(Self::parse_expression(tokens)?);
          },
          WonTokenKind::EndStatement => {},
          kind => {
            return Err(ParserError::UnexpectedToken(kind, WonTokenKind::EndStatement))
          }
        }

        Ok(Statement::Assignement { storage, identifier, ty, expression })
      },
      WonTokenKind::Assign => {
        Ok(Statement::Assignement { storage, identifier, ty: None, expression: Some(Self::parse_expression(tokens)?) })
      },
      WonTokenKind::EndStatement => {
        Ok(Statement::Assignement { storage, identifier, ty: None, expression: None })
      },
      kind => {
        Err(ParserError::UnexpectedToken(kind, WonTokenKind::EndStatement))
      }
    }
  }

  fn parse_expression<'src>(tokens: &mut Peekable<IntoIter<Token<'src, WonTokenKind>>>) -> Result<SpanContainer<'src, Expression>, ParserError<'src, WonTokenKind>> {
    let mut result = Err(ParserError::UnexpectedEOF);

    // Need to actually loop here eventually, once I have expression comprised of more than one token
    //while tokens.peek().is_some() {
      let token = tokens.next().unwrap();
      let kind = token.kind;
      result = match kind {
        WonTokenKind::True | WonTokenKind::False => Ok(SpanContainer::from_token(token, Expression::Literal(LiteralExprKind::Bool))),
        WonTokenKind::Number => Ok(SpanContainer::from_token(token, Expression::Literal(LiteralExprKind::Integer))),
        kind => Err(ParserError::ExpectedStatement(kind))
      };
    //}

    // Will also need to properly check for EndStatement in the loop
    Self::expect_token(tokens.next(), WonTokenKind::EndStatement)?;

    result
  }

  fn parse_type<'src>(next: Option<Token<'src, WonTokenKind>>) -> Result<SpanContainer<'src, Type>, ParserError<'src, WonTokenKind>> {
    let identifier: Token<'src, WonTokenKind> = Self::expect_token(next, WonTokenKind::Identifier)?;

    Ok(match identifier.span.get() {
      "bool" => SpanContainer::from_token(identifier, Type::Bool),
      content => { return Err(ParserError::TypeNotFound(content)); }
    })
  }

  fn expect_token<'src>(next: Option<Token<'src, WonTokenKind>>, kind: WonTokenKind) -> Result<Token<'src, WonTokenKind>, ParserError<'src, WonTokenKind>> {
    let token = next.ok_or(ParserError::UnexpectedEOF)?;
    
    if token.kind == kind {
      Ok(token)
    } else {
      Err(ParserError::UnexpectedToken(token.kind, kind))
    }
  }
}
