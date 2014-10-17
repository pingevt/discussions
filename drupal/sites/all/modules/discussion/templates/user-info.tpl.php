<?php
$account = $element['#account'];
?>

  <div id="user-info-block">

    <div class="btn-group user-admin-links">
      <button type="button" class="btn btn-xs btn-default dropdown-toggle" data-toggle="dropdown">
        <i class="fa fa-fixed fa-cog"></i>
      </button>
      <?php
        $list = array(
          '#theme' => 'item_list',
          '#items' => array(
            'profile' => l('Edit Profile', 'user/' . $account->uid . '/edit', array(
              'attributes' => array(
                //'role' => 'button',
                //'class' => array('btn', 'btn-default', 'btn-xs', 'btn-block'),
              ),
            )),
            'sign_out' => l('Sign Out', 'user/logout', array(
              'attributes' => array(
                //'role' => 'button',
                //'class' => array('btn', 'btn-default', 'btn-xs', 'btn-block'),
              ),
            )),
          ),
          '#attributes' => array(
            'class' => array('dropdown-menu', 'dropdown-menu-right'),
            'role' => 'menu',
          ),
        );
        print render($list);
      ?>
    </div>

    <div class="user-img">
      <?php //print theme('user_picture', array('account' => $account, 'style_name' => 'user_img_normal'));

        if (!empty($account->picture)) {
          if (is_numeric($account->picture)) {
            $account->picture = file_load($account->picture);
          }
          if (!empty($account->picture->uri)) {
            $filepath = $account->picture->uri;
          }
        }
        elseif (variable_get('user_picture_default', '')) {
          $filepath = variable_get('user_picture_default', '');
        }

        $alt = t("@user's picture", array('@user' => format_username($account)));
        print theme('image_style', array('style_name' => 'user_img_normal', 'path' => $filepath, 'alt' => $alt, 'title' => $alt));
      ?>
    </div>

    <h2><?php print $user->name; ?></h2>
  </div>
  <div class="clearfix"></div>
