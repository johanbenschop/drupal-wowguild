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

$result = db_query('SELECT * FROM {wowguildmembers}'. tablesort_sql($header));

while ($row = db_fetch_object($result)) {
  $full_path = drupal_get_path('module', 'wowguild');
  $img_race = '<img src="'. $full_path .'/images/icons/'. $row->raceId .'-'. $row->genderId .'.gif" /> ';
  $img_class = '<img src="'. $full_path .'/images/icons/'. $row->classId .'.gif" /> ';

  $rows[] = array(
      array('data' => l($row->name, "character/$row->name")),
      array('data' => $row->level),
      array('data' => $img_race . $names_arr['race'][$row->raceId]),
      array('data' => $img_class . $names_arr['class'][$row->classId]),
      array('data' => $row->rank)
  );
}

$display = theme_table($header, $rows);
$page_limitnum = "20";
$output .= theme('table', $header, $rows);
$output .= theme('pager', NULL, $page_limitnum);
print $output;
?>