extern crate proc_macro;
extern crate quote;

use proc_macro::TokenStream;
use quote::quote;

#[proc_macro_derive(Component)]
pub fn component_derive(input: TokenStream) -> TokenStream {
  let ast: syn::DeriveInput = syn::parse(input).unwrap();
  let name = &ast.ident;
  quote! {
    impl Component for #name {
      //const ID: u32 = p1::ecs::get_component_id::<#name>().unwrap();
    }
  }.into()
}
