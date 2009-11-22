<?php
// $Id$
/**
 * Template to display the Recruitment Block.
 *
 * Fields available:
 * $names_arr: an array that contains the humanreadeble names, see documentation fore more info.
 * $data: an array that contains the information of what classes are open.
 */
  $names_arr = wowguild_arr_names();
  $data = _wowguild_recruitment();
?>
<div id="wowguild_main">
<?php foreach ($names_arr['class'] as $class) {
  echo "". $class ."<br/>";
} ?>
</div>

<div id="wowguild_sidebar">
<?php foreach ($names_arr['class'] as $class) {
  $class = str_replace(" ", "_", $class);
  echo $names_arr['recruitment'][$data->$class];
  echo "<br/>";
} ?>
</div>
<div id="wowguild_apply">
<?php echo l('Submit Application!', 'apply');  ?>
</div>
