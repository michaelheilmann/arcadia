      <h2 id="structure">3 Structure of an asset</h2>
      <p>
      An ADL <em>asset</em> is a top-level DDL map describing a named
      <em>definition</em>. A caller may load several assets; such a batch is not
      itself one ADL asset. An asset has a String <code>type</code>, a String
      <code>name</code>, and the entries required or permitted by its type.
      </p>
      <p>
      The following DDL map is an example of a complete top-level asset:
      </p>
      <pre>
{ type: "Color", name: "Colors.Red", red: 255, green: 0, blue: 0 }
      </pre>
      <p>
      The two declaration contexts differ in their requirements for the
      <code>name</code> entry. Both require a String <code>type</code>, and
      each type specifies its own remaining entries:
      </p>
      <table>
        <caption>Definition entries by declaration context</caption>
        <thead><tr><th scope="col">Entry</th><th scope="col">Top-level asset</th><th scope="col">Inline definition</th></tr></thead>
        <tbody>
          <tr><td><code>type</code></td><td>Required String</td><td>Required String</td></tr>
          <tr><td><code>name</code></td><td>Required String</td><td>Forbidden</td></tr>
          <tr><td>Type-specific entries</td><td>As specified for the selected type</td><td>Identical rules for the selected type</td></tr>
        </tbody>
      </table>
      <p>
      The <code>type</code> selects a registered ADL definition type (section 8).
      An unknown type is a semantical error. The name identifies the top-level
      definition and must be unique within the application loading the assets.
      </p>

      <h3 id="structure-inline-definitions">3.1 Inline definitions and declaration context</h3>
      <p>
      An <em>inline
      definition</em> is a DDL map that describes an ADL definition as the value
      of an entry explicitly allowing this representation. It has the same type
      and type-specific semantics as a top-level definition but is not globally
      addressable. A nested DDL map is not automatically an inline definition.
      </p>
      <p>
      Example: a complete <code>UI.Button</code> asset containing an
      inline <code>Color</code> definition:
      </p>
      <pre>
{
  type: "UI.Button", name: "Buttons.Start",
  left: 0, bottom: 0, width: 100, height: 32, text: "Start",
  backgroundColor: { type: "Color", red: 255, green: 0, blue: 0 }
}
      </pre>
      <p>
      A top-level definition must contain <code>name</code> of type String.
      An inline definition must not contain a <code>name</code> entry at all;
      <code>name: ""</code> is also invalid. Both must contain a String
      <code>type</code> naming an ADL definition type permitted by section 8.
      A missing, non-String, or unknown <code>type</code> is invalid in either
      context. A type not permitted as an ordinary ADL definition does not
      become permitted merely because it appears inline.
      </p>
      <p>
      A named and an inline definition of the same ADL type have identical
      type-specific semantics: the same required and optional entries, value
      constraints, defaults, and interpretation and linking rules for their
      contents. Their declaration contexts differ only in naming, visibility,
      and containment. In particular, an inline definition does not have a
      restricted or alternative form of its type-specific schema.
      </p>

      <h3 id="structure-definition-valued-entries">3.2 Definition-valued entries and opt-in</h3>
      <p>
      A <em>definition-valued entry</em> is an entry whose specification accepts
      a String reference to a named definition, an inline definition map, or both.
      Each such entry's type-specific specification determines the permitted
      representation forms and the allowed ADL definition type or type family.
      An inline map is valid only when that entry explicitly permits it and the
      map's <code>type</code> is compatible with the entry's expected type.
      </p>
      <p>
      Allowing an inline definition in one entry does not allow it in any other
      reference-valued entry, even when the two entries expect the same ADL type.
      For this extension, only the five <code>UI.Button</code> color entries
      documented in section 6 gain the inline alternative. All other entries
      retain their current forms, including reference-only entries, ordinary
      strings, and lists of named references.
      </p>
      <pre>
// Alternative values in separate UI.Button assets:
backgroundColor: "Colors.Red" // reference to named Color
backgroundColor: { type: "Color", red: 255, green: 0, blue: 0 } // inline Color
      </pre>
      <p>
      These are alternative entry values, not two entries in one DDL map;
      duplicate entry names remain invalid as defined by DDL. A nested map
      containing <code>type</code> in an entry not permitting inline definitions
      is interpreted according to that entry's existing rules, not reclassified
      as an ADL definition.
      </p>

      <h3 id="structure-names-references">3.3 Names, visibility, and references</h3>
      <p>
      A <em>named reference</em> is a String in an entry explicitly specified as
      referencing a named definition. Where the entry's rules prescribe linking,
      the String is retained as a name during reading and resolved during linking.
      The target must exist in the global collection of loaded definitions and
      satisfy the entry's required type; absence or type mismatch is a linking
      error. An ordinary String is not implicitly a reference. Existing entries
      that retain a definition name without resolving it during linking keep that
      type-specific behavior.
      </p>
      <p>
      Inline definitions are not entered into the global definitions collection,
      introduce no local or global name, and cannot be addressed by a String
      reference. No hidden public identifier, qualified nested name, or local
      reference namespace is introduced. Inline definitions may contain named
      references wherever their type-specific entries permit them, and such
      references obey the same rules as in a top-level definition.
      </p>

      <h3 id="structure-validation-linking">3.4 Reading, containment, and linking</h3>
      <p>
      During reading, the containing entry determines whether the supplied
      value's DDL kind is permitted. For an inline map, reading must validate
      both the containing entry's allowed ADL type and the inline definition's
      type-specific entries under the rules of section 3.1. A forbidden
      <code>name</code>, a wrong ADL type, missing required entries, unsupported
      entries, or invalid values are semantical errors during reading. Invalid
      inline definitions must not be discarded or replaced with defaults.
      A reading error in an inline definition is an error in its containing asset.
      </p>
      <p>
      An inline definition is <em>semantically contained</em> by the immediately
      enclosing definition. It remains reachable through that definition while
      its semantic content is needed, but is not independently published in the
      global definitions collection. The containment relationship does not
      prescribe an allocation technique, object layout, C pointer type, or
      garbage-collection strategy.
      </p>
      <p>
      Linking a definition must include the linking required for all inline
      definitions it contains, recursively. References appearing in an inline
      definition are processed according to that type's ordinary linking rules.
      A linking failure in an inline definition is a linking failure of its
      containing definition, propagating through any enclosing definitions.
      Whether named definitions are declared before or after the containing
      asset does not change the existing rule that named references are resolved
      during linking against the loaded definitions.
      </p>
      <p>
      No new reference-cycle or graph-cycle policy is introduced. Existing
      type-specific linking restrictions and the graph-composition rules in
      section 9 remain in force. This extension does not require whole-batch
      transactional reading, failure rollback, or any particular linking algorithm.
      </p>

      <h3 id="structure-conformance">3.5 Distinct occurrences, compatibility, and conformance</h3>
      <p>
      Each inline definition map occurrence denotes a distinct logical
      definition occurrence, even if another occurrence has the same type and
      values. Equal contents do not imply identical definition identity or
      automatic sharing. The language introduces no structural equality,
      automatic interning, canonicalization, or deduplication of inline
      definitions. Implementations may share physical storage only where this
      cannot change observable ADL behavior. Authors share a definition
      explicitly by declaring it with a name and referring to that name.
      </p>
      <p>
      An omitted optional entry retains its existing default or fallback,
      unless the entry is explicitly required by its type's specification.
      For <code>UI.Button</code>, <code>backgroundColor</code> is required. An
      omitted optional button state color uses the same logical ADL Color
      definition as the background, whether the latter is a named reference
      or an inline definition. Fallback creates no additional definition
      occurrence. This does not restrict copying or converting Color component
      values into consumer representations.
      </p>
      <p>
      The following cases define acceptance or rejection of this
      extension. Unless otherwise stated, each inline Color occurs inside one
      of the five enabled <code>UI.Button</code> entries in section 6. The table
      specifies language behavior, not exact diagnostic text. A rejected read
      or link need not roll back previously loaded assets.
      </p>
      <table>
        <caption>Anonymous inline-definition conformance cases</caption>
        <thead><tr><th scope="col">Case</th><th scope="col">Required result</th></tr></thead>
        <tbody>
          <tr><td>Button with required String backgroundColor referencing a named Color</td><td>Accepted; resolve and type-check the reference during linking</td></tr>
          <tr><td>Button with all four optional state colors omitted</td><td>Accepted; all four states use the same logical definition as backgroundColor</td></tr>
          <tr><td>Button without backgroundColor, including when a state color is present</td><td>Semantical error during reading; no implicit Color or RGB default</td></tr>
          <tr><td>Button with backgroundRed, backgroundGreen, or backgroundBlue</td><td>Semantical error during reading; legacy entries are forbidden, even when backgroundColor is present</td></tr>
          <tr><td>Inline Color in any of the five enabled Button entries</td><td>Accepted; no global registration</td></tr>
          <tr><td>Named reference and inline Color in different entries of one Button</td><td>Accepted; each preserves its specified semantics</td></tr>
          <tr><td>Inline Color with omitted alpha</td><td>Accepted; alpha defaults to 255</td></tr>
          <tr><td>Inline Color with explicit alpha 0</td><td>Accepted; alpha remains 0</td></tr>
          <tr><td>Inline Color with explicit alpha 255</td><td>Accepted; alpha remains 255</td></tr>
          <tr><td>Omitted any of the four state colors with an inline background Color</td><td>Fallback uses the same logical background definition; no new definition occurrence</td></tr>
          <tr><td>Two separate inline Color maps with equal contents</td><td>Distinct logical occurrences; no implicit merging or global name</td></tr>
          <tr><td>Top-level Color without name</td><td>Semantical error during reading</td></tr>
          <tr><td>Inline Color with name, including an empty String</td><td>Semantical error during reading</td></tr>
          <tr><td>Inline Color with non-String name</td><td>Semantical error during reading; name is forbidden regardless of its value</td></tr>
          <tr><td>Inline map with missing or non-String type</td><td>Semantical error during reading</td></tr>
          <tr><td>Inline map with unknown type</td><td>Semantical error during reading</td></tr>
          <tr><td>Inline map of a registered but incompatible ADL type</td><td>Semantical error during reading</td></tr>
          <tr><td>Inline Color missing an RGB component</td><td>Semantical error during reading</td></tr>
          <tr><td>Inline Color with an out-of-range or wrong-kind component</td><td>Semantical error during reading</td></tr>
          <tr><td>Inline Color with an unsupported entry</td><td>Semantical error during reading</td></tr>
          <tr><td>Typed inline map supplied to a reference-only entry</td><td>Rejected according to that entry's existing schema</td></tr>
          <tr><td>Ordinary nested DDL map in an entry already accepting maps but not inline definitions</td><td>Retains its existing interpretation; not registered or linked as an inline ADL definition</td></tr>
          <tr><td>String reference to absent named definition in an enabled color entry</td><td>Linking error</td></tr>
          <tr><td>String reference to named definition of the wrong type</td><td>Linking error</td></tr>
          <tr><td>Graph composition without separately enabled inline fields</td><td>Existing graph-local naming, rewriting, and linking behavior unchanged</td></tr>
        </tbody>
      </table>
      <p>
      The recursive-linking rule in section 3.4 also governs any future
      explicitly permitted inline definition type that contains named references.
      Such references must be processed using that type's ordinary linking rules,
      and linking failures must propagate to the containing definition. This
      conditional requirement is not part of the initial Color conformance matrix.
      </p>
