<h1>CosmoCXX - The core library of CSpaceEngine</h1>
<p>This is the core module of CSpaceEngine, contains basic defines and functions</p>
<h2>Features</h2>
<h3>Math Functions</h3>
<p>Custom math functions for double precision and quaduple precision(Unused).</p>

<p>
	Notice: Basic math functions is from multiple libraries, and some of them maybe commercial products.
	In either event, it is copyrighted by the author. What you see here may be used freely but it comes with no support or guarantee.
</p>

<p>Function list:</p>
<table align="center">
	<tr>
		<th>Function</th>
		<th>Description</th>
		<th>Source</th>
		<th>Source License</th>
		<th>Precision</th>
	</tr>
	<tr><th colspan = 5 align = "center">E X P</td></th>
	<tr>
		<td>exp(x)</td>
		<td>Exponential Function e^x</td>
		<td>GLibC</td>
		<td>LGPL 2.1</td>
		<td>double and quaduple</td>
	</tr>
	<tr><th colspan = 5 align = "center">L O G</th></tr>
	<tr>
		<td>lb(x) (Unused)</td>
		<td>Binary Logarithm</td>
		<td>GLibC(double) / Cephes Math Library(quaduple)</td>
		<td>LGPL 2.1 / poorly specified</td>
		<td>double and quaduple</td>
	</tr>
	<tr>
		<td>ln(x)</td>
		<td>Natural Logarithm</td>
		<td>GLibC(double) / Stephen L. Moshier(quaduple)</td>
		<td>LGPL 2.1</td>
		<td>double and quaduple</td>
	</tr>
	<tr>
		<td>log(x)</td>
		<td>Common Logarithm based on 10</td>
		<td>Cephes Math Library</td>
		<td>poorly specified</td>
		<td>double and quaduple</td>
	</tr>
	<tr>
		<td>log(base, x)</td>
		<td>Logarithm to custom base</td>
		<td> ~ </td>
		<td> ~ </td>
		<td>double and quaduple</td>
	</tr>
	<tr><th colspan = 5 align = "center">P O W E R</th></tr>
	<tr>
		<td>pow(x, power)</td>
		<td>Power function</td>
		<td>IBM Accurate Mathematical Library(double)</td>
		<td>GPL 2.0?</td>
		<td>double and quaduple</td>
	</tr>
	<tr>
		<td>sqrt(x)</td>
		<td>Square root</td>
		<td>IBM Accurate Mathematical Library(double)</td>
		<td>GPL 2.0?</td>
		<td>double and quaduple</td>
	</tr>
	<tr>
		<td>cbrt(x)</td>
		<td>Cube root</td>
		<td>GLibC(double) / Cephes Math Library(quaduple)</td>
		<td>GPL 2.0? / poorly specified</td>
		<td>double and quaduple</td>
	</tr>
	<tr>
		<td>yroot(deg, x)</td>
		<td>nth root</td>
		<td> ~ </td>
		<td> ~ </td>
		<td>double and quaduple</td>
	</tr>
	<tr><th colspan = 5 align = "center">TRIGONOMETRY</th></tr>
	<tr>
		<td>sin(x)</td>
		<td>Sine</td>
		<td>OpenCV(angle) / IBM Accurate Mathematical Library(radians)</td>
		<td>Apache 2.0 / GPL 2.0?</td>
		<td>double</td>
	</tr>
	<tr>
		<td>cos(x)</td>
		<td>Cosine</td>
		<td>OpenCV(angle) / IBM Accurate Mathematical Library(radians)</td>
		<td>Apache 2.0 / GPL 2.0?</td>
		<td>double</td>
	</tr>
	<tr>
		<td>tan(x)</td>
		<td>Tangent</td>
		<td>IBM Accurate Mathematical Library(radians)</td>
		<td>GPL 2.0?</td>
		<td>double</td>
	</tr>
	<tr>
		<td>ctg(x)</td>
		<td>Cotangent</td>
		<td> ~ </td>
		<td> ~ </td>
		<td>double</td>
	</tr>
	<tr>
		<td>sec(x)</td>
		<td>Secant</td>
		<td> ~ </td>
		<td> ~ </td>
		<td>double</td>
	</tr>
	<tr>
		<td>csc(x)</td>
		<td>Cosecant</td>
		<td> ~ </td>
		<td> ~ </td>
		<td>double</td>
	</tr>
	<tr>
		<td>arcsin(x)</td>
		<td>Arcsine</td>
		<td>IBM Accurate Mathematical Library</td>
		<td>GPL 2.0?</td>
		<td>double</td>
	</tr>
	<tr>
		<td>arccos(x)</td>
		<td>Arccosine</td>
		<td>IBM Accurate Mathematical Library</td>
		<td>GPL 2.0?</td>
		<td>double</td>
	</tr>
	<tr>
		<td>arctan(x)</td>
		<td>Arctangent</td>
		<td>Stephen L. Moshier</td>
		<td>LGPL 2.1</td>
		<td>double</td>
	</tr>
	<tr>
		<td>arcctg(x)</td>
		<td>Arccotangent</td>
		<td> ~ </td>
		<td> ~ </td>
		<td>double</td>
	</tr>
	<tr>
		<td>arcsec(x)</td>
		<td>Arcsecant</td>
		<td> ~ </td>
		<td> ~ </td>
		<td>double</td>
	</tr>
	<tr>
		<td>arccsc(x)</td>
		<td>Arccosecant</td>
		<td> ~ </td>
		<td> ~ </td>
		<td>double</td>
	</tr>
</table>

<h3>GL Types</h3>

<p>Types from GLSL, and convertable to STL types. Their usage is almost same as the types in GLSL.</p>

<table>
	<tr>
		<th>Type</th>
		<th>description</th>
		<th>derivatives</th>
	</tr>
	<tr>
		<td>gl::gl_vec2&ltgenType&gt</td>
		<td>2-component vector</td>
		<td>vec2, ivec2, bvec2, uvec2</td>
	</tr>
	<tr>
		<td>gl::gl_vec3&ltgenType&gt</td>
		<td>3-component vector</td>
		<td>vec3, ivec3, bvec3, uvec3</td>
	</tr>
	<tr>
		<td>gl::gl_vec4&ltgenType&gt</td>
		<td>4-component vector</td>
		<td>vec4, ivec4, bvec4, uvec4</td>
	</tr>
	<tr>
		<td>gl::basic_matrix&lt_Ty, _Columns, _Lines&gt</td>
		<td>basic matrix witf sustom size, support linear algebra calculations.</td>
		<td>mat2, mat2x2, mat2x3, mat2x4, mat3x2, mat3, mat3x3, mat3x4, mat4x2, mat4x3, mat4, mat4x4</td>
	</tr>
</table>