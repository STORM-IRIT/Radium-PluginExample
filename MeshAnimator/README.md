# Mesh animator Radium Plugin example

This Plugin is an example of how to animate mesh vertices position or normal.

When the user pick a vertex on mesh (see the picking doc of radium),
The UI of this plugins displays info for this vertex and provides two
checkboxes enabling the animation of its position and/or normal.

When one of these checkbox is checked, a KeyFramedValue bound to the corresponding
attribute is created and given to the Timeline.
When it is unchecked, the KeyFramedValue if removed from the Timeline and destroyed.

Then, the user can select the KeyFramedValue he want to edit and define new keyframes.
When adding a new keyframe, its value would just be incremented by a small amount
(for example, if the vertex position is (1,1,1) before adding the keyframe,
the new keyframe will be (1+eps,1+eps,1+eps)).
