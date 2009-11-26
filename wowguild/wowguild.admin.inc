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
 * Page that contains the settings controls of the
 * Guild module.
 */
function page_wowguild_settings() {
  $out .= drupal_get_form('settings_form');
  $out .= wowguild_url('guild-stats');

  return $out;
}

function settings_form($form_state) {
  $names_arr = wowguild_arr_names();
  $default = _wowguild_get();

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic information'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => _wowguild_ifcollapsed($default),
    '#disabled' => _wowguild_ifcollapsed($default),
  );

  $form['basic']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $default->name,
    '#size' => 30,
    '#maxlength' => 64,
    '#description' => t('Enter your guilds name'),
    '#required' => TRUE,
    '#disabled' => _wowguild_ifcollapsed($default),
  );

  $form['basic']['realm'] = array(
    '#type' => 'textfield',
    '#title' => t('Realm'),
    '#default_value' => $default->realm,
    '#size' => 30,
    '#maxlength' => 64,
    '#description' => t('Enter the name of your realm'),
    '#required' => TRUE,
    '#disabled' => _wowguild_ifcollapsed($default),
  );

  $form['basic']['region'] = array(
    '#type' => 'select',
    '#title' => t('Region'),
    '#default_value' => $default->region,
    '#options' => $names_arr['region'],
    '#description' => t('Select the region of your realm'),
    '#required' => TRUE,
    '#disabled' => _wowguild_ifcollapsed($default),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Retrive and save'),
    '#disabled' => _wowguild_ifcollapsed($default)
  );

  return $form;
}

function settings_form_validate($form, &$form_state) {
  $url = wowguild_url(
    array(
      'type' => 'guild-stats',
      'region' => $form_state['values']['basic']['region'],
      'realm' => $form_state['values']['basic']['realm'],
      'name' => $form_state['values']['basic']['name']
    )
  );

  $fetched = _wowguild_fetch_armory($url);

  if ($fetched['error'] == FALSE) {
    if ($fetched['data']->guildInfo == '') {
      form_set_error('', t('Guild not found in the Armory.'));
    }
  }
  else {
      form_set_error('', t("The Armory is not accessible."));
  }
}

function settings_form_submit($form, &$form_state) {
  $url = wowguild_url(
    array(
      'type' => 'guild-stats',
      'region' => $form_state['values']['basic']['region'],
      'realm' => $form_state['values']['basic']['realm'],
      'name' => $form_state['values']['basic']['name']
    )
  );

  $fetched = _wowguild_fetch_armory($url);
  $arr = ($fetched['data']->xpath("//character[@rank='0']"));
  $guildleader = $arr[0]['name'];

  db_query(
    'INSERT INTO {wowguild} (name, realm, region, faction, members, battleGroup, guildLeader)'
     ."VALUES ('%s', '%s', '%s', %d, %d, '%s', '%s')",
    check_plain($fetched['data']->guildInfo->guildHeader->attributes()->name),
    check_plain($fetched['data']->guildInfo->guildHeader->attributes()->realm),
    check_plain($form_state['values']['basic']['region']),
    check_plain($fetched['data']->guildInfo->guildHeader->attributes()->faction),
    check_plain($fetched['data']->guildInfo->guildHeader->attributes()->count),
    check_plain($fetched['data']->guildInfo->guildHeader->attributes()->battleGroup),
    check_plain($guildleader)
  );

    $guild_settings = array(
      'name' => check_plain($fetched['data']->guildInfo->guildHeader->attributes()->name),
      'realm' => check_plain($fetched['data']->guildInfo->guildHeader->attributes()->realm),
      'region' => check_plain($form_state['values']['basic']['region']),
      'faction' => check_plain($fetched['data']->guildInfo->guildHeader->attributes()->faction),
      'members' => check_plain($fetched['data']->guildInfo->guildHeader->attributes()->count),
      'battleGroup' => check_plain($fetched['data']->guildInfo->guildHeader->attributes()->battleGroup),
      'guildLeader' => check_plain($guildleader)
    );

  variable_set('wowguid_settings', $guild_settings);
    
  $characters = $fetched['data']->xpath("//character");
  foreach ($characters as $character) {
    db_query(
      'INSERT INTO {wowguildmembers} (name, level, rank, achPoints, genderId, raceId, classId)'
       ."VALUES ('%s', %d, %d, %d, %d, %d, %d)",
      check_plain($character->attributes()->name),
      check_plain($character->attributes()->level),
      check_plain($character->attributes()->rank),
      check_plain($character->attributes()->achPoints),
      check_plain($character->attributes()->genderId),
      check_plain($character->attributes()->raceId),
      check_plain($character->attributes()->classId)
    );
    db_query(
      'INSERT INTO {wowguildprofessions} (name)'
       ."VALUES ('%s')",
      check_plain($character->attributes()->name)
    );
  }
  drupal_set_message( t('Confirmed and retrived information from the Armory.') );
}

/**
 * Page that contains the settings controls of the
 * reqruits.
 */
function page_wowguild_recruitment() {
  $out .= drupal_get_form('recruitment_form');

  return $out;
}

function recruitment_form($form_state) {
  $names_arr = wowguild_arr_names();
  $data = _wowguild_recruitment();

  $form['recruitment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recruitment'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
  );

  foreach ($names_arr['class'] as $class) {
    $class2 = str_replace(" ", "_", $class); // This is needed because the class Death Knight is two words and so incompatible with the database.
    $form['recruitment'][$class2] = array(
      '#type' => 'select',
      '#title' => t($class .'s'),
      '#default_value' => $data->$class2,
      '#options' => $names_arr['recruitment'],
      '#description' => t('Select whether the recruitment on '. $class .'s is open or not.'),
      '#required' => TRUE,
    );
  }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

function recruitment_form_submit($form, &$form_state) {
  db_query(
    'UPDATE {wowguildrecruitment} SET Warrior=%d, Paladin=%d, Hunter=%d, Roque=%d, Priest=%d, Death_Knight=%d, Shaman=%d, Mage=%d, Warlock=%d, Druid=%d',
    $form_state['values']['recruitment']['Warrior'],
    $form_state['values']['recruitment']['Paladin'],
    $form_state['values']['recruitment']['Hunter'],
    $form_state['values']['recruitment']['Roque'],
    $form_state['values']['recruitment']['Priest'],
    $form_state['values']['recruitment']['Death_Knight'],
    $form_state['values']['recruitment']['Shaman'],
    $form_state['values']['recruitment']['Mage'],
    $form_state['values']['recruitment']['Warlock'],
    $form_state['values']['recruitment']['Druid']
  );
  drupal_set_message( t('Confirmed and retrived information from the Armory.') );
}
