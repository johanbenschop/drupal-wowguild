<?php
// $Id$

/**
 * @file
 * Module for fetching data from GoodReads.com.
 * This module provides block content retrieved from a GoodReads.com
 * bookshelf.
 * @see http://www.goodreads.com
 */

/**
 * Page that displays the character sheet of an user.
 */
function page_character_sheet($character) {
  $char_arr = _wowguild_getmember($character);
  $names_arr = wowguild_arr_names();
  $content = theme('wowguild_sheet',
      check_plain($char_arr->name),
      check_plain($char_arr->level),
      check_plain($char_arr->genderId),
      check_plain($char_arr->classId),
      check_plain($char_arr->raceId),
      check_plain($names_arr['gender'][$char_arr->genderId]),
      check_plain($names_arr['class'][$char_arr->classId]),
      check_plain($names_arr['race'][$char_arr->raceId]),
      check_plain($char_arr->achPoints),
      check_plain($char_arr->prefix),
      check_plain($char_arr->suffix)
  );

  $url = wowguild_url(
      array(
      'type' => 'character-achievements',
      'region' => '',
      'realm' => '',
      'name' => $character
      )
  );
//  $url = check_plain($url);
  $http_result = _wowguild_fetch_armory($url);
  $xml = simplexml_load_string($http_result->data);
  for ($i = 0; $i <= 4; $i++) {
    $content .= theme('wowguild_achievement',
        $xml->achievements->summary->achievement[$i]['points'],
        substr($xml->achievements->summary->achievement[$i]['dateCompleted'], 2, 8),
        $xml->achievements->summary->achievement[$i]['title'],
        $xml->achievements->summary->achievement[$i]['desc']
    );
  }
  return $content;
}

/**
 * Page that contains the settings controls of the
 * Guild module.
 */
function page_wowguild_application() {
  $out = t('Fill in these simple questions and submit once you are done?');
  $out .= drupal_get_form('application_form');

  return $out;
}


/**
 * Form for the page_wowguild_application() function.
 */
function application_form($form_state) {
  if (empty($form_state['storage']['step'])) {
    // Setting step to our default.
    $form_state['storage']['step'] = 1;
  }

  switch ($form_state['storage']['step']) {
    case 1:
      $form['application'] = array(
          '#type' => 'fieldset',
          '#title' => t('Submit application'),
          '#tree' => TRUE,
          '#collapsible' => TRUE,
      );

      $form['application']['charactername'] = array(
          '#type' => 'textfield',
          '#title' => t('Character name'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('What is the name of your character?'),
//          '#required' => TRUE,
      );

      $form['application']['realname'] = array(
          '#type' => 'textfield',
          '#title' => t('Real name'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('What is your real name?'),
//          '#required' => TRUE,
      );


      $form['application']['email'] = array(
          '#type' => 'textfield',
          '#title' => t('E-Mail address'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('What is your valid e-mail address? (Your password and acount name will be sent to this address!)'),
//          '#required' => TRUE,
      );

      $form['application']['country'] = array(
          '#type' => 'textfield',
          '#title' => t('Country'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('In which country do you live in?'),
//          '#required' => TRUE,
      );

      $form['application']['aboutyou'] = array(
          '#type' => 'textarea',
          '#title' => t('Tell us about yourself?'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('Can you tell us somethings about yourself?'),
//          '#required' => TRUE,
      );

      $form['application']['whyapply'] = array(
          '#type' => 'textarea',
          '#title' => t('What makes you speciel enough to join our guild?'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('Do you have Teamspeak installed on your PC?'),
//          '#required' => TRUE,
      );
      break;

    case 2:
      $form['application'] = array(
          '#type' => 'fieldset',
          '#title' => t('Submit application'),
          '#tree' => TRUE,
          '#collapsible' => TRUE,
      );

      $form['application']['charactername'] = array(
          '#type' => 'textfield',
          '#title' => t('Character name'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('What is the name of your character?'),
//          '#required' => TRUE,
      );

      $form['application']['level'] = array(
          '#type' => 'textfield',
          '#title' => t('Character name'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('What is the name of your character?'),
//          '#required' => TRUE,
      );

      $form['application']['class'] = array(
          '#type' => 'textfield',
          '#title' => t('Character name'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('What is the name of your character?'),
//          '#required' => TRUE,
      );

      $form['application']['race'] = array(
          '#type' => 'textfield',
          '#title' => t('Character name'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('What is the name of your character?'),
//          '#required' => TRUE,
      );

      $form['application']['talents'] = array(
          '#type' => 'textfield',
          '#title' => t('Character name'),
          '#size' => 30,
          '#maxlength' => 64,
          '#description' => t('What is the name of your character?'),
//          '#required' => TRUE,
      );
      break;
  }

  // Show the 'Previous' button only on the second page.
  if ($form_state['storage']['step'] == 2) {
    $form['previous'] = array(
        '#type' => 'submit',
        '#value' => 'Previous'
    );
  }

  // Show the 'Next' button only on the first page.
  if ($form_state['storage']['step'] == 1) {
    $form['next'] = array(
        '#type' => 'submit',
        '#value' => 'Next'
    );
  }

  // Show the 'Submit' button on the last page.
  if ($form_state['storage']['step'] == 2) {
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit'
    );
  }
  return $form;
}

function application_form_submit($form, &$form_state) {
  // Save the values for the current step into the storage array.
  $form_state['storage']['values'][$form_state['storage']['step']] = $form_state['values'];

  // Check the button that was clicked and action the step change.
  if ($form_state['clicked_button']['#id']=='edit-previous') {
    $form_state['storage']['step']--;
  }
  elseif ($form_state['clicked_button']['#id']=='edit-next') {
    $form_state['storage']['step']++;
  }
  elseif ($form_state['clicked_button']['#id']=='edit-submit') {

    db_query(
    'INSERT INTO {wowguild_applicants} (nid, name, realname, email, country, aboutyou, whyapply)'
     ."VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s')",
    $test0,
    $form_state['values']['application']['charactername'],
    $form_state['values']['application']['realname'],
    $form_state['values']['application']['email'],
    $form_state['values']['application']['country'],
    $form_state['values']['application']['aboutyou'],
    $form_state['values']['application']['whyapply']
  );

    drupal_set_message( t('Confirmed and retrived information from the Armory.') );
  }
  // Tell Drupal we are redrawing the same form.
  $form_state['rebuild'] = TRUE;
}

/**
 * Page that displays the character sheet of an user.
 */
function page_wowguild_character() {
  return theme('wowguild_roster');
}

/**
 * Page that displays the proffesions of the characters with current skill.
 */
function page_wowguild_crafting() {
  $names_arr = wowguild_arr_names();

  $header = array(
      array('data' => 'Name', 'sort' => 'asc'),
      array('data' => 'Skill', 'sort' => 'asc')
  );

  $result = db_query('SELECT * FROM {wowguildprofessions}'. tablesort_sql($header));
  //$caption = 'TEST';
  // Puts the result from the sql query into seperate arrays for use with the table.
  // Meaning of numbers like 755, 186 and 182 can be found in the documentation
  // under 'profession codes'.
  while ($row = db_fetch_object($result)) {
    switch ($row->profession0id) {
      case 755:
        $profs755[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 186:
        $profs186[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 182:
        $profs182[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 773:
        $profs753[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 164:
        $profs164[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 202:
        $profs202[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 165:
        $profs165[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 393:
        $profs393[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 171:
        $profs171[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 333:
        $profs333[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
      case 197:
        $profs197[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession0value)
        );
        break;
    }

    switch ($row->profession1id) {
      case 755:
        $profs755[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 186:
        $profs186[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 182:
        $profs182[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 773:
        $profs753[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 164:
        $profs164[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 202:
        $profs202[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 165:
        $profs165[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 393:
        $profs393[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 171:
        $profs171[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 333:
        $profs333[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
      case 197:
        $profs197[] = array(
            array('data' => l($row->name, "character/$row->name")),
            array('data' => $row->profession1value)
        );
        break;
    }

  }

  $display .= '<table><tr>';
  $display .= '<th>Crafting</th>';
  $display .= '<th>Gathering</th>';
  $display .= '<th>Secondary</th></tr>';
  $display .= '<tr>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs164, $attributes = array() , 'Blacksmithing') .'</td>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs186, $attributes = array() , 'Mining') .'</td>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs, $attributes = array() , 'Cooking') .'</td>';
  $display .= '</tr>';

  $display .= '<tr>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs333, $attributes = array() , 'Enchanting') .'</td>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs182, $attributes = array() , 'Herbalism') .'</td>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs, $attributes = array() , 'First Aid') .'</td>';
  $display .= '</tr>';

  $display .= '<tr>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs202, $attributes = array() , 'Engineering') .'</td>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs393, $attributes = array() , 'Skinning') .'</td>';
  $display .= '</tr>';

  $display .= '<tr>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs773, $attributes = array() , 'Inscription') .'</td>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs171, $attributes = array() , 'Alchemy') .'</td>';
  $display .= '</tr>';

  $display .= '<tr>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs755, $attributes = array() , 'Jewelcrafting') .'</td>';
  $display .= '</tr>';

  $display .= '<tr>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs165, $attributes = array() , 'Leatherworking') .'</td>';
  $display .= '</tr>';

  $display .= '<tr>';
  $display .= '<td id="wowguild_crafting">'. theme_table($header, $profs197, $attributes = array() , 'Tailoring') .'</td>';
  $display .= '</tr>';

  $display .= '</table>';

  return $display;
}

/**
 * Imports information such as professions from a character to the database.
 * It is possible that the inmports simply fails due to the Armory having errors.
 */
function page_wowguild_import($character) {
  $output = t('Fill in these simple questions and submit once you are done?');

  $url = wowguild_url(
      array(
      'type' => 'character-sheet',
      'name' => $character
      )
  );

  $http_result = _wowguild_fetch_armory($url);
  $xml = simplexml_load_string($http_result->data);

  db_query(
      "UPDATE {wowguildprofessions} SET profession0id=%d, profession0value=%d, profession1id=%d, profession1value=%d WHERE name='%s'",
      check_plain($xml->characterInfo->characterTab->professions->skill[0]->attributes()->id),
      check_plain($xml->characterInfo->characterTab->professions->skill[0]->attributes()->value),
      check_plain($xml->characterInfo->characterTab->professions->skill[1]->attributes()->id),
      check_plain($xml->characterInfo->characterTab->professions->skill[1]->attributes()->value),
      check_plain($character)
  );

  return $output;
}


