<?php
// $Id$
/**
 * Template to display the Recruitment Block.
 *
 * Fields available:
 * $names_arr: an array that contains the humanreadeble names, see documentation fore more info.
 * $data: an array that contains the information of what classes are open.
 */
  $names_arr = wimd_arr_names();
  $data = variable_get('wimd_reqruiting', NULL)
?>
<div id="wimd_main">
<?php foreach ($names_arr['class'] as $class) {
  echo "". $class ."<br/>";
} ?>
</div>

<div id="wimd_sidebar">
<?php foreach ($names_arr['class'] as $class) {
  $class = str_replace(" ", "_", $class);
  echo $names_arr['recruitment'][$data[$class]];
  echo "<br/>";
} ?>
</div>
<div id="wimd_apply">
<?php echo l('Submit Application!', 'application');  ?>
</div>
