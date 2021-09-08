#! /bin/bash
#
#  Last modified:  Nov 12, 2020
#
#  This script must be executed in the grading/ directory, in the testing
#  structure described in the accompanying README file:
#
#   README                      SO READ IT!!
#   compare                     utility used by the grading script
#   dev/                        empty directory; develop your solution here
#   gisdata/                    directory holding GIS data files used in grading
#   scripts/                    directory holding command files used in grading
#   logs/                       directory holding reference output logs used in grading
#   grading/                    directory for testing your solution
#      gradeC08.sh              bash script to automate grading procedure
#      <tar file holding your solution goes here>                         
#   suppliedcode/               directory holding supplied code you may use;
#                                  copy files to the dev/ directory as needed;
#                                  include those files in your tar
#      StringHashTable.h           StringHashTable header/implementation files
#      StringHashTable.o           
#      nextField.h                 nextField() header/implementation files
#      nextField.o
#
#  The directory must also contain a tar file containing all of the files that
#  are necessary in order to compile your solution.
#
#  We recommend that you name that file yourPID.tar, using your VT email PID as
#  the first part of the file name, as the Curator will do.
#
#  Invocation:  ./gradeC08.sh <name of your tar file>
#               e.g., ./gradeC08.sh wmcquain.C08.1.tar
#
#  If you followed the suggested naming convention, there will be a file named
#  yourPID.txt that contains the results of testing.  The script will:
#
#   - Extract your tar file to ./tempDir, then copy your .c, .h, and supplied
#     files you've included to the ./build directory and attempt to compile.
#     The command used to compile your submission, and any resulting error or
#     warning messages should be echoed to file buildLog.txt.
#   - Copy the executable (named c08_yourPID) and the supplied GIS data files
#     into the testing directory.
#   - Attempt to run your executable on each of the supplied script files,
#     producing log files named stulogXX.txt, where XX is the sequence number
#     of the script file that was used.
#   - Use the compare tool to compare your log files to the supplied reference
#     log files and generate a score for each.
#   - Run valgrind to check for memory-related errors, using one of the
#     supplied script files.
#   - Compile results into a single report file, yourPID.txt
#
#  Score data should be found in the second section of the final report file,
#  looking something like this (the file names and scores may vary):
#
#		>>Scores from testing<<
#		1 >> Score from stuLog01.txt:  50 / 50
#		2 >> Score from stuLog02.txt:  180 / 180
#		3 >> Score from stuLog03.txt:  120 / 120
#		4 >> Score from stuLog04.txt:  435 / 435
#     . . .
#
#  Valgrind results should be summarized in the third section of the final
#  report.  That should be followed by detailed results from the compare
#  tool (valuable in determining why you may have lost points on one of
#  the tests), then by the detailed valgrind log, and then by the messages
#  from the build process.  If errors occur, some of these sections may be
#  omitted.
#
#  Alternate invocation:  ./gradeC08.sh -clean
#
#  This will remove the files created by an earlier run of the grading script;
#  it's useful if you just want to start with a pristine environment.
#
#  Name of grading package file and comparison tool
compTool="compare"

#  Names for directories
buildDir="build"
tempDir="tempdir"
scriptDir="../scripts"
logDir="../logs"
gisDir="../gisdata"

#   Names for log files created by this script:
headerLog="header.txt"
buildLog="buildLog.txt"
testLog="details.txt"
scoreLog="scores.txt"

#   Name for the executable
exeName="c08"

#  Names for driver-generated test files
testCases="TestData.txt"
results="Results.txt"

#   Delimiter to separate sections of report file:
Separator="================================================================================"

############################################# fn to check for tar file
#                 param1:  name of file to be checked
isTar() {

   mimeType=`file -b --mime-type $1`
   if [[ $mimeType == "application/x-tar" ]]; then
     return 0
   fi
   if [[ $mimeType == "application/x-gzip" ]]; then
     return 0
   fi
   return 1
}

##################################### fn to extract token from file name
#                 param1: (possibly fully-qualified) name of file
#  Note:  in production use, the first part of the file name will be the
#         student's PID
#
getPID() { 

   fname=$1
   # Strip off any leading path info
   fname=${fname##*/}
   # Extract first token of file name
   sPID=${fname%%.*}
}

##################################### fn to extract token from file name
clean() {
     echo "Removing earlier test files..."
     rm -Rf *.txt $buildDir $tempDir $exeName $compTool
     rm -Rf "script*.txt"
     rm -Rf "NM_*.txt" "VA_*.txt" "Mixed*.txt"
}
   
############################################################ Validate command line

# Verify number of parameters
   if [[ $# -ne 1 ]]; then
      echo "You must specify the name of your tar file (or -clean)."
      exit 1
   fi
   
# See if user selected the cleaning option
  if [[ $1 == "-clean" ]]; then
     clean
     exit 0
  fi

# Verify presence of named file and that it's a tar
   sourceFile="$1"
   sourceFile=${sourceFile##*/}
   if [ ! -e $sourceFile ]; then
      echo "The file $sourceFile does not exist."
      exit 2
   fi
   isTar "$sourceFile"
   if [[ ! $? -eq 0 ]]; then
      echo "The file $sourceFile is not a tar file."
      exit 2
   fi

############################################################ Get student's PID
   
# Extract first token of submitted file name (student PID when we run this)
   getPID $sourceFile
   exeName=$exeName"_$sPID"
   summaryLog="$sPID.txt"
   
# Initiate header for grading log
   echo "Grading:  $1" > $headerLog
   echo -n "Time:     " >> $headerLog
   echo `date` >> $headerLog
   echo >> $headerLog
   
############################################################ Prepare for build
  
# Create build log file:
   echo "Executing gradeC08.sh..." > $buildLog
   echo >> $buildLog
   
# Create build directory:
   echo "Creating build subdirectory" >> $buildLog
   echo >> $buildLog
# Create build directory if needed; empty it if it already exists
   if [[ -d $buildDir ]]; then
      rm -Rf "./$buildDir/*"
   else
      mkdir $buildDir
   fi
   
# Unpack student's tar file to a temp directory
   mkdir ./$tempDir
   tar xf $sourceFile -C ./$tempDir
   echo "Student's tar file contains:" >> $buildLog
   tar tvf $sourceFile >> $buildLog
   echo >> $buildLog
# Copy student's C source files (.c and .h) into build directory
   echo "Copying the student's .c and .h files to the build directory:" >> $buildLog
   cp ./$tempDir/*.c $buildDir
   if compgen -G "./$tempDir/*.h" >> /dev/null; then
      cp ./$tempDir/*.h $buildDir
   fi
# If present, copy supplied binaries into build directory
   if [[ -e ./$tempDir/StringHashTable.o ]]; then
      cp ./$tempDir/StringHashTable.o $buildDir
   fi
   if [[ -e ./$tempDir/nextField.o ]]; then
      cp ./$tempDir/nextField.o $buildDir
   fi
   echo >> $buildLog
   ls -l $buildDir >> $buildLog
   echo >> $buildLog

# Move to build directory
   cd ./$buildDir
   
####################################################### Compile student's submission

# Set build command:
   buildCMD="gcc -o $exeName -std=c11 -Wall -W -ggdb3 -lm *.c"
   if [[ -e StringHashTable.o ]]; then
      buildCMD=$buildCMD" StringHashTable.o"
   fi
   if [[ -e nextField.o ]]; then
      buildCMD=$buildCMD" nextField.o"
   fi
   
# Build the driver; save output in build log
   echo "Compiling the submitted files:" >> ../$buildLog
   echo "    $buildCMD" >> ../$buildLog
   $buildCMD >> ../$buildLog 2>&1
   echo >> ../$buildLog

# Verify existence of executable
   if [[ ! -e $exeName ]]; then
      echo "Build failed; the file $exeName does not exist" >> ../$buildLog
      echo $Separator >> ../$buildLog
      cp ../$headerLog ../$sPID.txt
      cat ../$buildLog >> ../$sPID.txt
      exit 7
   fi
   if [[ ! -x $exeName ]]; then
      echo "Permissions error; the file $exeName is not executable" >> ../$buildLog
      echo $Separator >> ../$buildLog
      cp ../$headerLog ../$sPID.txt
      cat ../$buildLog >> ../$sPID.txt
      exit 8
   fi

   echo "Build succeeded..." >> $buildLog
   
# Move executable up to test directory and return there
   echo "Moving the executable $exeName to the test directory." >> $buildLog
   mv ./$exeName .. >> $buildLog
   cd .. >> $buildLog
   
# Delimit this section of the report file:
   echo $Separator >> $buildLog

############################################################ Perform tests
   
# Initiate test Log
   echo "Begin testing..." > $testLog
   
# Copy the comparison tool into the test directory:
   if [[ -e ../$compTool ]]; then
      cp ../$compTool .
   else
      echo "ERROR:  $compTool not found!" 
      echo "Please correct test setup as described in spec." 
      exit 10
   fi
   
# Verify the presence of the script and reference log directories:
   if [[ ! -e $scriptDir ]]; then
      echo "ERROR:  script directory $scriptDir not found!"
      echo "Please correct test setup as described in spec." 
      exit 10 
   fi
   if [[ ! -e $logDir ]]; then
      echo "ERROR:  log directory $logDir not found!"
      echo "Please correct test setup as described in spec." 
      exit 10 
   fi
   
# Copy the GIS data files into the current directory
   if [[ -d $gisDir ]]; then
      echo "Copying GIS data files to test directory" >> $testLog
      cp $gisDir/* .
   else
      echo "ERROR:  GIS data directory $gisDir not found!"
      echo "Please correct test setup as described in spec." 
      exit 10 
   fi
   
# Count the test scripts
   echo "Running tests using the following scripts:" >> $testLog
   echo >> $testLog
   ls -l $scriptDir/script*.txt >> $testLog
   nCases=`ls $scriptDir/script*.txt | wc -w`

# Perform testing
   index=1
   killed="no"
   
   ############################## run the GIS test cases
   while [ "$index" -le "$nCases" ]
    do
      script="script0$index.txt"
      refLog="reflog0$index.txt"
      stuLog="stulog0$index.txt"

      echo -n "$Separator " >> $testLog
      echo "Testing with $script" >> $testLog

      # Write-protect the supplied script and log files
      chmod a-w $scriptDir/$script
      chmod a-w $logDir/$refLog

      # Run student's gis soln on current test file
      echo "Running student soln on $scriptDir/$script" >> $testLog
      echo >> $testLog
      killed="no"
      timeout -s SIGKILL 60 ./$exeName $scriptDir/$script $stuLog >> $testLog 2>&1
		timeoutStatus="$?"
		if [[ $timeoutStatus -eq 124 || $timeoutStatus -eq 137 ]]; then
			echo "The test of your solution timed out after 60 seconds." >> $testLog
			echo "Valgrind testing will NOT be done." >> $testLog
			killed="yes"
		elif [[ $timeoutStatus -eq 134 ]]; then
			echo "The test of your solution was killed by a SIGABRT signal." >> $testLog
			echo "Possible reasons include:" >> $testLog
			echo "    - a segfault error" >> $testLog
			echo "    - a serious memory access error" >> $testLog
			echo "Valgrind testing will NOT be done." >> $testLog
			killed="yes"
		fi

      # Remove write-protection from test files
      chmod u+w $scriptDir/$script
      chmod u+w $logDir/$refLog

      # Verify existence of a nonempty student log file
      if [ ! -s $stuLog ]; then
         echo "The log file $stuLog was not produced or is empty" >> $testLog
         echo >> $testLog
         ((index +=1))
         continue
      fi

      # Run comparator on current reference and student logs
      echo "Running compare on $refLog and $stuLog" >> $testLog
      ./$compTool $index $logDir/$refLog $stuLog >> $testLog
      compLog="compare0$index.txt"
      echo "$compLog results:" >> $testLog
      cat $compLog >> $testLog
      echo >> $testLog
      echo $Separator >> $testLog
      echo >> $testLog
      
      ((index +=1))
   done

############################################################ Run valgrind test

   if [[ $killed = "no" ]]; then
	   #  Full valgrind output is in $vgrindLog
	   #  Extracted counts are in $vgrindIssues
	   vgrindLog="vgrindLog.txt"
      
	   echo "Running valgrind test..." >> $vgrindLog
	   vgrindSwitches=" --leak-check=full --show-leak-kinds=all --log-file=$vgrindLog --track-origins=yes -v"
	   scoreResultsLog2="ScoresValgrind.txt"
	   tmpVgrind="tmpVgrind.txt"
	   script="script04.txt"
	   stuLog="stuvgrindLog.txt"
	   timeout -s SIGKILL 60 valgrind $vgrindSwitches ./$exeName $scriptDir/$script ./$stuLog >> $tmpVgrind 2>&1
		timeoutStatus="$?"
		# echo "timeout said: $timeoutStatus"
		if [[ $timeoutStatus -eq 124 || $timeoutStatus -eq 137 ]]; then
			echo "The test of your solution timed out after 60 seconds." >> $testLog
			echo "Valgrind testing will NOT be completed." >> $testLog
			killed="yes"
		elif [[ $timeoutStatus -eq 134 ]]; then
			echo "The test of your solution was killed by a SIGABRT signal." >> $testLog
			echo "Possible reasons include:" >> $testLog
			echo "    - a segfault error" >> $testLog
			echo "    - a serious memory access error" >> $testLog
			echo "Valgrind testing will NOT be completed." >> $testLog
			killed="yes"
		fi
	   if [[ -s $tmpVgrind ]]; then
	      cat $tmpVgrind >> $vgrindLog
	   fi
	   
	   # Accumulate valgrind error counts
	   if [[ -e $vgrindLog ]]; then
	      vgrindIssues="vgrind_issues.txt"
	      echo "Valgrind issues:" > $vgrindIssues
	      grep "in use at exit" $vgrindLog >> $vgrindIssues
	      grep "total heap usage" $vgrindLog >> $vgrindIssues
	      grep "definitely lost" $vgrindLog >> $vgrindIssues
	      echo "Invalid reads: `grep -c "Invalid read" $vgrindLog`" >> $vgrindIssues
	      echo "Invalid writes: `grep -c "Invalid write" $vgrindLog`" >> $vgrindIssues
	      echo "Uses of uninitialized values: `grep -c "uninitialised" $vgrindLog`" >> $vgrindIssues
         echo >> $vgrindIssues
	   else
	      echo "Error running Valgrind test." >> $testLog
	   fi
   fi
   
############################################################ File report
# Create summary file
   
   # Write header to summary log
   cat "$headerLog" > $summaryLog
   echo $Separator >> $summaryLog
   echo >> $summaryLog
   
   # Gather scores from the test log produced earlier
   grep "Score from" $testLog > $scoreLog
   echo ">>Scores from testing<<" >> $summaryLog
   cat $scoreLog >> $summaryLog
   echo >> $summaryLog
   echo $Separator >> $summaryLog
   echo >> $summaryLog
   
   # If student program was not killed, write Valgrind summary into log
   if [[ $killed = "no" ]]; then
	   echo "Summary of valgrind results:" >> $summaryLog
	   echo >> $summaryLog
	   cat $vgrindIssues >> $summaryLog
	   echo $Separator >> $summaryLog
   fi
   
   # Write test output to summary log
   echo "Detailed results from testing:" >> $summaryLog
   echo >> $summaryLog
   cat $testLog >> $summaryLog
   echo >> $summaryLog
   
   # If student program was not killed, write Valgrind details into log
   if [[ $killed = "no" ]]; then
	   echo "Details from valgrind check:" >> $summaryLog
	   echo >> $summaryLog
	   cat $vgrindLog >> $summaryLog
	   echo $Separator >> $summaryLog
   fi
   
   # Write build log into summary
   echo "Results from $buildLog" >> $summaryLog
   cat $buildLog >> $summaryLog
   echo >> $summaryLog

exit 0
