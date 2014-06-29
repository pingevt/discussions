<?php
$url = $_GET['q'];
$query_params = drupal_get_query_parameters();
if (isset($query_params['page'])) unset($query_params['page']);
?>

<div class="text-center">
  <ul class="pagination pagination-sm">

    <?php
      // Previous
      if ($page > 0):
        print '<li>' . l('&laquo;', $url, array('html' => TRUE, 'query' => array_merge($query_params, array('page' => 0)))) . '</li>';
        print '<li>' . l('&lsaquo;', $url, array('html' => TRUE, 'query' => array_merge($query_params, array('page' => ($page-1))))) . '</li>';
      endif;
    ?>

    <?php
    for ($i=0; $i <= $max; $i++) {
      $class = array();

      if ($i == 0) {
        $class[] = 'first';
      }

      if ($i == 0 && $max > 4 && $page > 2) {
        $class[] = 'bump';
      }

      if ($i == $page) {
        $class[] = 'active';
      }

      if ($i == $max) {
        $class[] = 'last';
      }

      if ($i == $max && $max > 4 && $page < ($max - 2)) {
        $class[] = 'bump';
      }

      if ($max <= 4 || $page == $i || in_array($i, array(0, $max, ($page+1), ($page-1)))) {
        print '<li' . drupal_attributes(array('class' => $class)) . '>';
        print  l(($i+1), $url, array('html' => TRUE, 'query' => array_merge($query_params, array('page' => $i))));
        print '</li>';
      }
    }


    ?>
    <?php
      // Next
      if ($page != $max):
        print '<li>' . l('&rsaquo;', $url, array('html' => TRUE, 'query' => array_merge($query_params, array('page' => ($page+1))))) . '</li>';
        print '<li>' . l('&raquo;', $url, array('html' => TRUE, 'query' => array_merge($query_params, array('page' => ($max))))) . '</li>';
      endif;
    ?>
  </ul>
</div>
