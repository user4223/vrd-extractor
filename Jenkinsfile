import org.jenkinsci.plugins.pipeline.modeldefinition.Utils

def checkForSkip() {
   if (currentBuild.result ==~ '(?i)^(failure)$') {
      echo "${STAGE_NAME} skipped\n"
      Utils.markStageSkippedForConditional(STAGE_NAME)
      return true
   }
   return false
}

def catchAndFail(Closure body) {
   try { body.call() } 
   catch (e) {
      echo "\n\n\n${e.toString()}\n\n\n"
      currentBuild.result = 'FAILURE'
   }   
}

node {
   properties([
      parameters([
         choice(
             choices:      ['Release', 'Debug', 'Coverage'].join('\n')
            ,name:         'buildConfiguration'
            ,description:  'Configuration of the build')
   ])])

   currentBuild.displayName = "${currentBuild.displayName}.${params.buildConfiguration}"

   checkout scm
   
   try {
      catchAndFail {
         stage('Build') {
            if (checkForSkip()) { return }
            sh """
               mkdir -p build
               cd build
               cmake -DCMAKE_BUILD_TYPE=${params.buildConfiguration} ../Source
               cmake --build .
            """
         }
      }

      catchAndFail {
         stage('Test') {
            if (checkForSkip()) { return }

            step ([$class: 'CopyArtifact', projectName: 'vrdextractor.images', filter: '*.tar']);

            sh """
               tar xvf vrdlib.test.images.tar --directory Source/vrdlib/test/
               cd bin
               ./vrdlib.test --gtest_output=xml:vrdlib.test.result.xml
            """
            
            archiveArtifacts artifacts: 'bin/vrdlib.test.result.xml', flattenDirectories: true
            
            step([$class: 'XUnitBuilder',
                      thresholds: [[$class: 'SkippedThreshold', failureThreshold:   '0']
                                  ,[$class: 'FailedThreshold',  failureThreshold:   '0']]
                     ,tools:      [[$class: 'GoogleTestType',   pattern:            'bin/vrdlib.test.result.xml']]
            ])
            
            if (params.buildConfiguration.equalsIgnoreCase('Coverage')) {
               sh """
                  cd build
                  gcovr -x -r ../ -e \".+(gmock|gtest).*[.](cpp|h)\$\" -e \".+(Test|TestBase|Mock|main)[.](cpp|h)\$\" > vrdlib.test.coverage.xml
                  """
               
               archiveArtifacts artifacts: 'build/vrdlib.test.coverage.xml', flattenDirectories: true
               
               step([$class: 'CoberturaPublisher'
                        ,coberturaReportFile: 'build/vrdlib.test.coverage.xml'
                        ,autoUpdateHealth:   false   ,autoUpdateStability: false
                        ,failUnhealthy:      false   ,failUnstable: false
                        ,maxNumberOfBuilds:  0
                        ,onlyStable:         false
                        ,zoomCoverageChart:  false])
            }
         }
      }
      
      catchAndFail {
         stage('Doxygen') {
            if (checkForSkip()) { return }
            
            sh 'doxygen'

            publishHTML (target: [
               allowMissing: false,
               alwaysLinkToLastBuild: false,
               keepAll: true,
               reportDir: 'documentation/html',
               reportFiles: 'index.html',
               reportName: "Doxygen Report"
            ])
         }
      }

      catchAndFail {
         stage('Deploy') {
            if (checkForSkip()) { return }
            // do
         }
      }
   }
   finally {
      deleteDir()
   }
}
