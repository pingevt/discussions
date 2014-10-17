<?php

/**
 * Override or insert variables into the maintenance page template.
 */
function discussions_preprocess_maintenance_page(&$vars) {
  // While markup for normal pages is split into page.tpl.php and html.tpl.php,
  // the markup for the maintenance page is all in the single
  // maintenance-page.tpl.php template. So, to have what's done in
  // discussions_preprocess_html() also happen on the maintenance page, it has to be
  // called here.
  discussions_preprocess_html($vars);
}

/**
 * Override or insert variables into the html template.
 */
function discussions_preprocess_html(&$vars) {
  // Add conditional CSS for IE8 and below.
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'weight' => 999, 'preprocess' => FALSE));
  // Add conditional CSS for IE7 and below.
  drupal_add_css(path_to_theme() . '/css/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'weight' => 999, 'preprocess' => FALSE));
  // Add conditional CSS for IE6.
  drupal_add_css(path_to_theme() . '/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'weight' => 999, 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the page template.
 */
function discussions_preprocess_page(&$vars) {
  global $user;

  drupal_add_js(path_to_theme() . '/bootstrap/js/dropdown.js');

  if ($user->uid == 0) {
    $vars['theme_hook_suggestions'] = array('page__login');
  }

  $vars['primary_local_tasks'] = $vars['tabs'];
  unset($vars['primary_local_tasks']['#secondary']);
  $vars['secondary_local_tasks'] = array(
    '#theme' => 'menu_local_tasks',
    '#secondary' => $vars['tabs']['#secondary'],
  );
}

/**
 * Implements hook_preprocess_node().
 */
function discussions_preprocess_node(&$variables) {

  if (isset($variables['submitted']) && $variables['submitted'] != '') {
    $variables['submitted'] = '<div class="submitted-date">' . date('n/j/y', $variables['node']->created) . '<br />' . date('h:ia', $variables['node']->created) . '</div>';
  }

  if ($variables['node']->type == 'discussion') {
    drupal_add_js(path_to_theme() . '/bootstrap/js/collapse.js');
    drupal_add_js(path_to_theme() . '/bootstrap/js/transition.js');
  }
}

/**
 * Implements hook_preprocess_comment().
 */
function discussions_preprocess_comment(&$variables) {
  $variables['submitted'] = '<div class="submitted-date">' . date('n/j/y', $variables['comment']->created) . '<br />' . date('h:ia', $variables['comment']->created) . '</div>';
}

function discussions_form_alter(&$form, &$form_state, $form_id) {
  _add_bootstrap_form_control($form);

  if ($form_id == 'user_login_block') {

  }

  if ($form_id == 'user_pass') {

  }
}

function _add_bootstrap_form_control(&$element) {
  $children = element_children($element, TRUE);

  if (count($children) > 0) {
    foreach ($children as $child_id) {

      if (isset($element[$child_id]['#type'])) {

        switch ($element[$child_id]['#type']) {
        case 'textarea':
        case 'textfield':
        case 'text_format':
        case 'date':
        case 'select':
        case 'password':
        case 'password_confirm':
        case 'submit':
          $element[$child_id]['#attributes']['class'][] = 'form-control';
          break;
        }
      }

      // Update children
      _add_bootstrap_form_control($element[$child_id]);
    }
  }
}

/**
 * Display the list of available node types for node creation.
 */
function discussions_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="clearfix">';
      $output .= '<span class="label">' . l($item['title'], $item['href'], $item['localized_options']) . '</span>';
      $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  else {
    $output = '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }
  return $output;
}

/**
 * Overrides theme_admin_block_content().
 *
 * Use unordered list markup in both compact and extended mode.
 */
function discussions_admin_block_content($variables) {
  $content = $variables['content'];
  $output = '';
  if (!empty($content)) {
    $output = system_admin_compact_mode() ? '<ul class="admin-list compact">' : '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="leaf">';
      $output .= l($item['title'], $item['href'], $item['localized_options']);
      if (isset($item['description']) && !system_admin_compact_mode()) {
        $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Override of theme_tablesort_indicator().
 *
 * Use our own image versions, so they show up as black and not gray on gray.
 */
function discussions_tablesort_indicator($variables) {
  $style = $variables['style'];
  $theme_path = drupal_get_path('theme', 'discussions');
  if ($style == 'asc') {
    return theme('image', array('path' => $theme_path . '/images/arrow-asc.png', 'alt' => t('sort ascending'), 'width' => 13, 'height' => 13, 'title' => t('sort ascending')));
  }
  else {
    return theme('image', array('path' => $theme_path . '/images/arrow-desc.png', 'alt' => t('sort descending'), 'width' => 13, 'height' => 13, 'title' => t('sort descending')));
  }
}

/**
 * Implements hook_css_alter().
 */
function discussions_css_alter(&$css) {
  // Use discussions's vertical tabs style instead of the default one.
  if (isset($css['misc/vertical-tabs.css'])) {
    $css['misc/vertical-tabs.css']['data'] = drupal_get_path('theme', 'discussions') . '/vertical-tabs.css';
    $css['misc/vertical-tabs.css']['type'] = 'file';
  }
  if (isset($css['misc/vertical-tabs-rtl.css'])) {
    $css['misc/vertical-tabs-rtl.css']['data'] = drupal_get_path('theme', 'discussions') . '/vertical-tabs-rtl.css';
    $css['misc/vertical-tabs-rtl.css']['type'] = 'file';
  }
  // Use discussions's jQuery UI theme style instead of the default one.
  if (isset($css['misc/ui/jquery.ui.theme.css'])) {
    $css['misc/ui/jquery.ui.theme.css']['data'] = drupal_get_path('theme', 'discussions') . '/jquery.ui.theme.css';
    $css['misc/ui/jquery.ui.theme.css']['type'] = 'file';
  }
}

function discussions_textarea($variables) {
  $element = $variables['element'];
  $element['#attributes']['name'] = $element['#name'];
  $element['#attributes']['id'] = $element['#id'];
  $element['#attributes']['cols'] = $element['#cols'];
  $element['#attributes']['rows'] = $element['#rows'];
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    $wrapper_attributes['class'][] = 'resizable';
  }
  if (!empty($element['#attributes']['rows']) && $element['#attributes']['rows'] > 5) {
    $element['#attributes']['rows'] = 5;
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';

  return $output;
}

/**
 * Implements hook_node_view_alter().
 */
function discussions_node_view_alter(&$build) {

  if ($build['#bundle'] == 'discussion' && in_array($build['#view_mode'], array('full', 'discussion_replies'))) {

    $build['#contextual_links'] = array(
      'node' => array(
        'node',
        array($build['#node']->nid),
      ),
    );
  }

  if ($build['#bundle'] == 'event' && in_array($build['#view_mode'], array('full', 'discussion_replies'))) {

    $build['#contextual_links'] = array(
      'node' => array(
        'node',
        array($build['#node']->nid),
      ),
    );
  }

  if ($build['#bundle'] == 'gallery' && in_array($build['#view_mode'], array('full', 'discussion_replies'))) {

    $build['#contextual_links'] = array(
      'node' => array(
        'node',
        array($build['#node']->nid),
      ),
    );
  }
  //dpm($build);
}

/**
 * Implements hook_comment_view_alter().
 */
function discussions_comment_view_alter(&$build) {
  /*
  $build['#contextual_links'] = array(
    'comment' => array(
      'comment',
      array($build['#comment']->cid),
    ),
  );
  */
}

function discussions_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="nav nav-tabs nav-primary">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="nav nav-pills nav-secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

function discussions_button($variables) {

  if ($variables['element']['#button_type'] == 'submit') {
    if ($variables['element']['#value'] == 'Save') {
      $variables['element']['#attributes']['class'][] = 'btn';
      $variables['element']['#attributes']['class'][] = 'btn-primary';
    }
    if ($variables['element']['#value'] == 'Delete') {
      $variables['element']['#attributes']['class'][] = 'btn';
      $variables['element']['#attributes']['class'][] = 'btn-warning';
    }
    if ($variables['element']['#value'] == 'Upload') {
      $variables['element']['#attributes']['class'][] = 'btn';
      $variables['element']['#attributes']['class'][] = 'btn-default';
    }
    if ($variables['element']['#value'] == 'Cancel') {
      $variables['element']['#attributes']['class'][] = 'btn';
      $variables['element']['#attributes']['class'][] = 'btn-warning';
    }
  }

  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }


  if ($variables['element']['#button_type'] == 'submit') {
    if ($variables['element']['#value'] == 'E-mail new password') {
      return '<div class="col-sm-6"><input' . drupal_attributes($element['#attributes']) . ' /></div>';
    }
    elseif (in_array($variables['element']['#value'], array('Save', 'Delete'))) {
      return '<div class="col-sm-4"><input' . drupal_attributes($element['#attributes']) . ' /></div>';
    }
    else {
      return '<input' . drupal_attributes($element['#attributes']) . ' />';
    }
  }
  else {
    return '<input' . drupal_attributes($element['#attributes']) . ' />';
  }
}

function discussions_menu_alter(&$items) {
  // Remove view from local tasks.
  $items['node/%node/view']['type'] = MENU_NORMAL_ITEM;
}

function discussions_image($variables) {
  $attributes = $variables['attributes'];
  $attributes['src'] = file_create_url($variables['path']);

  foreach (array('alt', 'title') as $key) {

    if (isset($variables[$key])) {
      $attributes[$key] = $variables[$key];
    }
  }

  if (isset($attributes['width'])) {
    unset($attributes['width']);
  }

  if (isset($attributes['height'])) {
    unset($attributes['height']);
  }

  return '<img' . drupal_attributes($attributes) . ' />';
}

function discussions_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];

  // Only output the list container and title, if there are any list items.
  // Check to see whether the block title exists before adding a header.
  // Empty headers are not semantic and present accessibility challenges.
  $output = '';
  if (isset($title) && $title !== '') {
    $output .= '<h3>' . $title . '</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    $i = 0;
    foreach ($items as $item) {
      $attributes = array();
      $children = array();
      $data = '';
      $i++;
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
      if (count($children) > 0) {
        // Render nested list.
        $data .= theme_item_list(array('items' => $children, 'title' => NULL, 'type' => $type, 'attributes' => $attributes));
      }
      if ($i == 1) {
        $attributes['class'][] = 'first';
      }
      if ($i == $num_items) {
        $attributes['class'][] = 'last';
      }
      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
    }
    $output .= "</$type>";
  }
  //$output .= '</div>';
  return $output;
}
