
sub listdir {  
 my @arr, $j = 0;  
 for ($i=0;$i<=$#_;$i++) {  
  if (-d $_[$i]) {  
   if (opendir($handle, $_[$i])) {  
    while ($entry = readdir($handle)) {  
     if (!($entry =~ m/^\.$/) and !($entry =~ m/^(\.\.)$/)) {   
      if (-d $_[$i]."\\$entry") {  # is a directory, push to @arr
       $arr[$j++] = $_[$i]."\\$entry";   
      }  
      else {   # is a file
      	if ($entry =~ /$.cpcfg$/) { # is a .cpcfg file, copy it
      		$cmd = "copy ";
      		$cmd .= $_[$i]."\\$entry ";
      		$cmd .= "C:\\ControlPanel\\debug\\bin\\config"."\\$entry";
      		print ("$cmd\n");
      		system($cmd);
      	}
      }
     }  
    }  
    closedir($handle);  
   }  
  }  
 }  
 if ($j>0) {  
  listdir (@arr);  
 }  
}

# create target directory if it doesn't exist
print ("Creating direcotry... C:\\ControlPanel\\debug\\bin\\config \n");
mkdir ("C:\\ControlPanel");
mkdir ("C:\\ControlPanel\\debug");
mkdir ("C:\\ControlPanel\\debug\\bin");
mkdir ("C:\\ControlPanel\\debug\\bin\\config");

# go through source directories recrusively
listdir (".\\src\\cpapplication");
listdir (".\\src\\cpplugins");