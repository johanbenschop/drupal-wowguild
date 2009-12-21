<?php
// $Id$

/**
 * Provides the wimd content type.
 * @file
 */

/**
 * Implements hook_node_info()
 */
function wimd_node_info() {
  return array(
    'wimd' => array(
      'module' => 'wimd',
      'name' => t('wimd'),
      'description' => t("A wimd of a person."),
      'has_title' => true,
      'title_label' => t('wimd of'),
      'has_body' => true,
      'body_label' => t('Overview'),
  	  //'module' => 'wimd',
    )
  );
}

/**
 * Implements hook_perm()
 */
function wimd_perm() {
  return array(
    'create wimd node',
    'edit wimd nodes',
    'delete wimd nodes',
  );
}

/**
 * Implements hook_access()
 */
function wimd_access($op, $node, $account) {
  switch ($op) {
  case 'create':
    return user_access('create wimd node', $account);
  case 'update':
    return user_access('edit wimd nodes', $account);
  case 'delete':
    return user_access('delete wimd nodes', $account);
  }
}

// ======================================
// Database:
// ======================================

/**
 * implements hook_insert()
 */
function wimd_insert($node) {
  
  if (!isset($node->life)) $node->life = '';
  if (!isset($node->works)) $node->life = '';
  
  db_query(
    'INSERT INTO {wimd} (vid, nid, dates, life, works) '
      ."VALUES (%d, %d, '%s', '%s','%s')", 
    $node->vid, 
    $node->nid, 
    $node->dates, 
    $node->life,
    $node->works
  );
}
/**
 * implements hook_update()
 */
function wimd_update($node) {
  if ($node->revision) { // Node is new or a revision. See node_example.module.
    wimd_insert($node);
  } 
  else {
    db_query("UPDATE {wimd} SET dates = '%s', life = '%s', works='%s' "
        ."WHERE vid = %d", 
      $node->dates, 
      $node->life, 
      $node->works,
      $node->vid
    );
  }
}
/**
 * implements hook_delete()
 */
function wimd_delete($node) {
  db_query('DELETE * FROM {wimd} WHERE nid = %d', $node->nid);
}

/**
 * Implements hook_nodeapi()
 * This handles deleting revisions.
 */
function wimd_nodeapi(&$node, $op, $teaser, $page) {
  if ($op == 'delete revision')
      db_query('DELETE * FROM {wimd} WHERE vid = %d', $node->vid);
}

// ======================================
// Creating Data:
// ======================================
/**
 * Implementation of hook_form()
 */
function wimd_form(&$node) {
  $type = node_get_types('type', $node);

  // Existing files: title (wimd of) and body (Overview)
  if ($type->has_title) {
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => check_plain($type->title_label),
      '#required' => true,
      '#default_value' => $node->title,
      '#weight' => -5
    );
  }

  if ($type->has_body) {
    $form['body_field'] = node_body_field(
        $node, 
        $type->body_label, 
        $type->min_word_count
      );
  }

  // Our custom fields: Dates, Life, Works.
  // (See FAPI docs for specification)
  $form['dates'] = array(
    '#type' => 'textfield',
    '#size' => 50,
    '#maxlengh' => 127,
    '#title' => t('Dates'),
    '#description' => t("Birth and death dates."),
    '#default_value' => isset($node->dates) ? $node->dates : '',
  );
  $form['life'] = array(
    '#type' => 'textarea',
    '#title' => t('Life'),
    '#cols' => 50,
    '#rows' => 5,
    '#description' => t("A description of this person's life."),
    '#default_value' => isset($node->life) ? $node->life : '',
  );
  $form['works'] = array(
    '#type' => 'textarea',
    '#title' => t('Works'),
    '#cols' => 50,
    '#rows' => 5,
    '#description' => t("An annotated bibliography of this person's works."),
    '#default_value' => isset($node->works) ? $node->works : '',
  );

  return $form;
}

// ======================================
// Getting Data:
// ======================================
/**
 * Implementation of hook_load()
 */
function wimd_load($node) {
  $result = db_query(
    'SELECT dates, life, works FROM {wimd} WHERE vid = %d',
    $node->vid
  );
  return db_fetch_object($result);
}
/**
 * Implementation of hook_view()
 */
function wimd_view($node, $teaser=false, $page=false) {
  $node = node_prepare($node, $teaser); // get it ready for display
  
  $dates = check_plain($node->dates);
  $life = check_markup($node->life);
  $works = check_markup($node->works);
  
  // Add theme stuff here
  $node->content['wimd_info'] = array(
    '#value' => theme('wimd_info', $dates, $life, $works),
    '#weight' => 1,
  );
  
  return $node;
}

// ======================================
// Theme:
// ======================================
/**
 * implements hook_theme()
 */
function wimd_theme() {
  return array(
    'wimd_info' => array(
      'template' => 'wimd_info',
      'arguments' => array(
        'dates' => null,
        'life' => null,
        'works' => null,
      ),
    ),
  );
}
