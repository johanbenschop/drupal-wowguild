<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


$names_arr = wowguild_arr_names();


$header = array(
    array('data' => 'Character', 'sort' => 'asc', 'field' => 'name'),
    array('data' => 'Level', 'sort' => 'asc', 'field' => 'level'),
    array('data' => 'Race', 'sort' => 'asc', 'field' => 'raceId'),
    array('data' => 'Class', 'sort' => 'asc', 'field' => 'classId'),
    array('data' => 'Guild Rank', 'sort' => 'asc', 'field' => 'rank')
);

$rows = array();
$sql = 'SELECT * FROM {wowguildmembers}';
$count = 20;
$query = pager_query($sql. tablesort_sql($header), $count);

while ($record = db_fetch_object($query)) {
  $full_path = drupal_get_path('module', 'wowguild');
  $img_race = '<img src="'. $full_path .'/images/icons/'. $record->raceId .'-'. $record->genderId .'.gif" /> ';
  $img_class = '<img src="'. $full_path .'/images/icons/'. $record->classId .'.gif" /> ';

  $rows[] = array(
      array('data' => l($record->name, "character/$record->name")),
      array('data' => $record->level),
      array('data' => $img_race . $names_arr['race'][$record->raceId]),
      array('data' => $img_class . $names_arr['class'][$record->classId]),
      array('data' => $record->rank)
  );
}

$output .= theme('table', $header, $rows);
print $output . theme('pager', $count);
?>