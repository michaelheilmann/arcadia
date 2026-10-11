#if !defined(ARCADIA_VISUALS_MODELRENDERER_H_INCLUDED)
#define ARCADIA_VISUALS_MODELRENDERER_H_INCLUDED

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.ModelRenderer", Arcadia_Engine_Visuals_ModelRenderer,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Visuals_Implementation_ModelRendererDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_ModelRenderer {
  Arcadia_Object _parent;
};

#endif // ARCADIA_VISUALS_MODELRENDERER_H_INCLUDED
