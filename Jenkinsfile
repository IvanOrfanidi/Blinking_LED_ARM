
pipeline {
    agent any
    stages {
        stage('submodule update') {
            steps {
                sh 'git submodule init'
                sh 'git submodule update'
            }
        }

        stage('build') {
            steps {
                sh 'bash compile.sh'
            }
        }

        stage('cppcheck') {
            steps {
                sh 'cppcheck --enable=all --xml --xml-version=2 -i ext  src 2> build/cppcheck.xml'
                sh 'cppcheck-htmlreport --source-encoding="iso8859-1" --title="project" --source-dir=. --report-dir=build --file=build/cppcheck.xml'
                publishHTML(
                    target: [
                      allowMissing: true,
                      alwaysLinkToLastBuild: false,
                      keepAll: false,
                      reportDir: 'build',
                      reportFiles: 'index.html',
                      reportName: 'CppCheck Report',
                      reportTitles: ''
                    ]
                )
            }
        }

        stage('artifacts') {
            steps {
                archiveArtifacts artifacts: "build/*.hex", onlyIfSuccessful: true
                archiveArtifacts artifacts: "build/*.bin", onlyIfSuccessful: true
            }
        }

        stage('clean') {
            steps {
                cleanWs()
            }
        }
    }
    options {
        // Хранить только одну удачную сбороку
        buildDiscarder(logRotator(numToKeepStr: '1'))
    }
}
