<?php
// $Id$
/**
 * Template to display a character page.
 *
 * Fields available:
 * $name: cleaned plain text string
 * $level: cleaned plain text string
 * $genderId: cleaned plain text string
 * $classId: cleaned plain text string
 * $raceId: cleaned plain text string
 * $gender: cleaned plain text string
 * $class: cleaned plain text string
 * $race: cleaned plain text string
 * $achPoints: cleaned plain text string
 */

  $full_path = drupal_get_path('module', 'wimd');
  if ($level >= 0) {
    $avatarlevel = "10";
  };
  if ($level >= 60) {
    $avatarlevel = "60";
  };
  if ($level >= 70) {
    $avatarlevel = "70";
  };
  if ($level >= 80) {
    $avatarlevel = "80";
  };
?>
<div id="wimd_sheet">

<div id="wimd_frame">
  <img src="<?php print $full_path;?>/images/<?php print $avatarlevel?>_Avatars/<?php print $genderId; print '-'; print $raceId; print '-'; print $classId;?>.gif" width="64" height="64" alt="<?php print $name;?>" title="<?php print $name;?>" />
  <div id="wimd_points"><?php print $achPoints;?></div>
</div>

<div id="wimd_names">
  <h id="wimd_prefix"><?php print $prefix;?></h>
  <h id="wimd_name"><?php print $name;?></h>
  <h id="wimd_prefix"><?php print $suffix;?></h>
</div>

<div id="wimd_info">
Level <?php print $level;?> <?php print $race;?> <?php print $class;?>
</div>
</div><div id="wimd_sheet"></div>
<h2>Latest achievments:</h2>



