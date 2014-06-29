<?php
$account = $element['#account'];
?>

  <div id="user-info-block">
    <div class="user-img">
      <?php print theme('user_picture', array('account' => $account, 'style_name' => 'user_img_normal')); ?>
    </div>
    <h2><?php print $user->name; ?></h2>
    <?php
      $list = array(
        '#theme' => 'item_list',
        '#items' => array(
          'profile' => l('Edit Profile', 'user/' . $account->uid . '/edit', array(
            'attributes' => array(
              'role' => 'button',
              'class' => array('btn', 'btn-default', 'btn-xs', 'btn-block'),
            ),
          )),
          'sign_out' => l('Sign Out', 'user/logout', array(
            'attributes' => array(
              'role' => 'button',
              'class' => array('btn', 'btn-default', 'btn-xs', 'btn-block'),
            ),
          )),
        ),
      );
      print render($list);
    ?>
  </div>
