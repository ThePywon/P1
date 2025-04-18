use proc_macro::TokenStream;
//use proc_macro2::TokenStream as TokenStream2;
use quote::quote;
use syn::{Fields, Ident};

#[proc_macro_derive(Component)]
pub fn component_derive(input: TokenStream) -> TokenStream {
  let ast: syn::DeriveInput = syn::parse(input).unwrap();
  let name = &ast.ident;
  quote! {
    impl Component for #name {}
  }
  .into()
}

#[proc_macro_derive(EventData)]
pub fn event_data_derive(input: TokenStream) -> TokenStream {
  let ast: syn::DeriveInput = syn::parse(input).unwrap();
  let name = &ast.ident;
  quote! {
    impl EventData for #name {
      type Item = TimeDelta;

      fn get_item(tick: &Tick/* , command_managers: _ */) -> Self::Item { tick.delta(&Tick::new()) }
    }
  }
  .into()
}

#[proc_macro_derive(TokenKind, attributes(meta))]
pub fn token_kind_derive(input: TokenStream) -> TokenStream {
  let ast: syn::DeriveInput = syn::parse(input).unwrap();
  let name = &ast.ident;
  let data_enum: syn::DataEnum = match ast.data {
    syn::Data::Enum(data) => data,
    _ => panic!("Failed to parse TokenKind, parsing data is not an enum."),
  };

  let mut match_arms_ident: Vec<&Ident> = Vec::new();
  let mut match_arms_fields: Vec<&Fields> = Vec::new();
  let mut match_arms_result: Vec<String> = Vec::new();
  for item in data_enum.variants.iter() {
    for attribute in item.attrs.iter() {
      if attribute.path().is_ident("meta") {
        match_arms_ident.push(&item.ident);
        match_arms_fields.push(&item.fields);
        match_arms_result.push(attribute.parse_args::<Ident>().unwrap().to_string());
        break;
      }
    }
  }

  quote! {
    impl TokenKind for #name {
      fn get_meta(&self) -> &str {
        match self {
          #(Self::#match_arms_ident #match_arms_fields => #match_arms_result,)*
        }
      }
    }

    impl std::fmt::Debug for #name {
      fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
          #(Self::#match_arms_ident #match_arms_fields => f.debug_struct(stringify!(#name::#match_arms_ident)),)*
        }.field("meta", &self.get_meta()).finish()
      }
    }
  }.into()
}
