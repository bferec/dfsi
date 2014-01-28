<?php

// ------------------------------------------------
// @(#) dfsiexport.form.inc
// @(#) modification des formulaires
// @(#) (c) Amael Hervoche
// @(#) 24/01/2014
// ------------------------------------------------

function dfsiexport_form_alter(&$form, &$form_state, $form_id) 
{
    if($form['#form_id']  == 'dfsiexport_ExportForm' ) //
    {
        $form['dfsiexport_choixExport'][1]['#default_value'] = 1;
    }
}
