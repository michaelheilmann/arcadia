<section class="cxx entity enumeration">
<h1 id="Arcadia_FileAccessMode">Arcadia_FileAccessMode</h1>
<my-signature><code>
typedef enum Arcadia_FileAccessMode Arcadia_FileAccessMode;
</code></my-signature>

<my-summary>
An enumeration of file access modes.
The enumeration elements can be combined.
</my-summary>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_FileAccessMode_Read</code></div>
    <div>
    Read access. Can be combined with <code>Arcadia_FileAccessMode_Write</code>.
    </div>
  </div>

  <div>
    <div><code>Arcadia_FileAccessMode_Write</code></div>
    <div>
    Write access. Can be combined with <code>Arcadia_FileAccessMode_Read</code>.
    </div>
  </div>

  <div>
    <div><code>Arcadia_FileAccessMode_ReadWrite</code></div>
    <div>
    Read and write access. Alias for <code>Arcadia_FileAccessMode_Read|Arcadia_FileAccessMode_Write</code> and for <code>Arcadia_FileAccessMode_WriteRead</code>.
    </div>
  </div>

  <div>
    <div><code>Arcadia_FileAccessMode_WriteRead</code></div>
    <div>
    Write and read access. Alias for <code>Arcadia_FileAccessMode_Write|Arcadia_FileAccessMode_Read</code> and for <code>Arcadia_FileAccessMode_ReadWrite</code>.
    </div>
  </div>

</section>

</section>
