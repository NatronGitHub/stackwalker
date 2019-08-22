<?php
// Generate index.xml for Breakdown

$folder = '/storage/ssd5/003/10550003/public_html/reports';
$input = '<?xml version="1.0" standalone="yes"?><reports></reports>';
$output = $folder . '/index.xml';

$xml = new SimpleXMLElement($input);
$files = glob($folder . '/*.dmp', GLOB_BRACE);
foreach($files as $file) {
    $info = pathinfo($file);
    $uuid =  basename($file, '.' . $info['extension']);
    $report = $xml->addChild('report');
    $report->addChild('uuid', $uuid);
    $report->addChild('dmp', sha1_file($folder . '/' . $uuid. '.dmp'));
    $report->addChild('txt', sha1_file($folder . '/' . $uuid. '.txt'));
}

$result = $xml->asXML();

$result_checksum = sha1($result);
$old_checksum = '';
if (file_exists($output)) {
    $old_checksum = sha1(file_get_contents($output));
}
if ($result_checksum != $old_checksum) {
    file_put_contents($output, $xml->asXML());
}
?>
