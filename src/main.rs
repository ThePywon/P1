mod ecs;
mod error;
mod event;
mod p1;
mod rendering;
mod utility;

extern crate macros;

use std::ffi::CString;

use ecs::{Component, Query};
use event::{builtin::Update, Event, IntervalListener};
use interpreted::{
  lexer::Lexer,
  parser::Parser,
  won::{lexer::WonLexer, parser::WonParser},
};
use macros::Component;
use p1::P1;
use serde::{Deserialize, Serialize};

use eval::{eval, Expr, Function};
use winit::window::Window;

#[derive(Serialize, Deserialize, Debug, Component, Clone, Copy)]
struct A {
  position: (i32, i32),
  scale: (i32, i32),
}

#[derive(Component)]
struct B();
#[derive(Component, Debug)]
struct C();
#[derive(Component)]
struct D();
#[derive(Component)]
struct E();
#[derive(Component)]
struct F();
#[derive(Component)]
struct G();
#[derive(Component)]
struct H();
#[derive(Component, Debug)]
struct I();

fn main() {
  let mut engine = P1::new().unwrap();
  let my_entity = engine.create_entity();
  let entity_b = engine.create_entity();
  let entity_c = engine.create_entity();
  //dbg!(my_entity);
  //dbg!(entity_b);
  //dbg!(entity_c);
  engine
    .add_component(
      my_entity,
      A {
        position: (1, 2),
        scale: (3, 4),
      },
    )
    .unwrap();
  engine.add_component(my_entity, C {}).unwrap();
  engine.add_component(my_entity, I {}).unwrap();

  let tokens = WonLexer::lex(
    b"
  let my_var = (2 as usize) as u32;;;;;;;;;;;
  let another_var = false;
  let undefined_var: bool;
  print(test, test2, test3);
  ",
  );
  //dbg!(&tokens);

  println!("{}", tokens[35].span);

  let ast = WonParser::parse(tokens);
  //dbg!(&ast);

  //let arc = engine.get_component::<A>(my_entity).unwrap();
  //dbg!(arc.downcast_ref::<A>());

  /*let system = |mut query: Query<(&mut A, (&C, &I))>, event: Event<Update>| {
    for (a, (c, i)) in query.iter_mut() {
      dbg!(&a);
      dbg!(&c);
      dbg!(&i);
      dbg!(event.get_data());
    }
  };*/

  // Need some way to describe the structure of objects
  // Strongly typed
  // Some way to export components?

  /*
    struct MyCustomComponent {
      property_name: type = some_def_val_at_construction;
      // what does an uninitialized property do when a component needs to be instantiated?

      fun function_name(arg_name: arg_type) -> return_type {
        if condition {

        } else if {

        } else {

        }
        return value;
      }
    }
  */

  //engine.register_system(system).unwrap();
}
