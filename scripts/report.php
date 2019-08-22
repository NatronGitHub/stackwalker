<?php
// Natron Crash Collector Reference (very simple edition)

// timezone should be set according to server
// SET_YOUR_TIMEZONE_HERE

// don't show any warnings or errors
error_reporting(0);

// get a timestamp
$date = date('Y-m-d H:i:s');

// whatever the php user can write to,
// some data might contain sensitive information
// so should be a "private" location
$target_dir = "/storage/ssd5/003/10550003/public_html/reports/";

// POST values from NatronCrashReporter
$target_product = $_POST["ProductName"];
$target_version = $_POST["Version"];
$target_comment = trim($_POST["Comments"]);
$target_gitcommit = $_POST["GitHash"];
$target_gitbranch = $_POST["GitBranch"];
$target_io_gitcommit = $_POST["IOGitHash"];
$target_misc_gitcommit = $_POST["MiscGitHash"];
$target_arena_gitcommit = $_POST["ArenaGitHash"];
$target_linuxversion = trim($_POST["LinuxVersion"]);
$target_contact = $_POST["Contact"];
$target_severity = $_POST["Severity"];
$target_glrenderer = $_POST["GLrenderer"];
$target_glversion = $_POST["GLversion"];
$target_glvendor = $_POST["GLvendor"];
$target_glshader = $_POST["GLshader"];
$target_glext = trim($_POST["GLext"]);
$target_features = $_POST['features'];
$target_guid = $_POST["guid"];
$target_file = $target_dir . basename($_FILES["upload_file_minidump"]["name"]);
$target_txt = $target_dir . basename($_FILES["upload_file_minidump"]["name"],".dmp") . ".txt";
$target_ip = $_SERVER['REMOTE_ADDR'];

// make sure comments don't mess up our json
$target_comment = str_replace("\n", '', $target_comment);
$target_comment = str_replace("\r", '', $target_comment);
$target_comment = str_replace("\r\n", '', $target_comment);

// gen json-ish string for embedding custom data onto a stackwalker generated json file
$target_content = '{"submitted_timestamp": "' .$date. '", "features": "' .$target_features. '", "gl_ext": "' .$target_glext. '", "gl_shader": "' .$target_glshader. '", "gl_vendor": "' .$target_glvendor. '", "gl_version": "' .$target_glversion. '", "gl_renderer": "' .$target_glrenderer. '", "user_contact": "' .$target_contact. '", "user_severity": "' .$target_severity. '", "linux_distro": "' .$target_linuxversion. '","git_commit": "' .$target_gitcommit. '", "git_branch": "' .$target_gitbranch. '", "io_commit": "' .$target_io_gitcommit. '", "misc_commit": "' .$target_misc_gitcommit. '", "arena_commit": "' .$target_arena_gitcommit. '", "client_ip": "' .$target_ip. '", "uuid": "' .$target_guid. '", "timestamp": "' .$date. '", "throttle_rate": 100, "dump_checksums": {"upload_file_minidump": "' .$target_guid. '"}, "Comments": "' .$target_comment. '", "ProductName": "' .$target_product. '", "legacy_processing": 0, "Version": "' .$target_version. '", "type_tag": "bp", "guid": "' .$target_guid. '"}';

// do some simple checks before saving the data
if (!empty($target_guid) &&
    !empty($target_product) &&
    !empty($target_version) &&
    !empty($target_file) &&
    !empty($target_txt)) {
  if (!file_exists($target_file) &&
      !file_exists($target_txt) &&
      $_FILES["upload_file_minidump"]["size"] < 900000 &&
      pathinfo($target_file, PATHINFO_EXTENSION)=="dmp") {
    // add more security checks here if needed!

    // save dump file
    move_uploaded_file($_FILES["upload_file_minidump"]["tmp_name"], $target_file);

    // save custom data from Natron in a txt file,
    // this file is embedded onto the final json produced
    // by stackwalker that can be read by breakdown
    file_put_contents($target_txt, $target_content);
  }
  // return guid (as expected by Natron)
  echo $target_guid;
}
?>
