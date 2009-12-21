<?php
// $Id$
/**
 * Template to display a character page.
 *
 * Fields available:
 * $name: cleaned plain text string
 * $members: cleaned plain text string
 * $realm: cleaned plain text string
 * $battlegroup: cleaned plain text string
 * $faction: cleaned plain text string
 * $guildleader: cleaned plain text string
 */

  $full_path = drupal_get_path('module', 'wimd')
?>
<div class="guild_stats">
Guild name: <?php print $name;?>
<br/>Members: <?php print $members;?>
<br/>Realm: <?php print $realm;?>
<br/>Battlegroup: <?php print $battlegroup;?>
<br/>Faction: <?php print $faction;?>
<br/>Guild Leader: <?php print $guildleader;?>
</div>

