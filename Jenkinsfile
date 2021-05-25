pipeline {
    agent {
        dockerfile true
    }

    environment {
        RTI_MIN_PACKAGE_URL = credentials('rti-min-package-url')
        RTI_INSTALLATION_PATH = "${WORKSPACE}"
    }

    stages {
        stage('Download') {
            steps {
                publishChecks conclusion: 'NONE', detailsURL: 'https://www.rti.com', name: 'Download Packages', 
                    status: 'IN_PROGRESS', summary: 'Download RTI Connext DDS libraries', title: 'Download Packages'

                sh 'python3 resources/ci_cd/linux_install.py'
            }

            post {
                success {
                    publishChecks detailsURL: 'https://www.rti.com', name: 'Download Packages',
                        summary: 'Downloads RTI Connext DDS libraries', title: 'Download Packages'
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: 'https://www.rti.com', name: 'Download Packages',
                        summary: 'Downloads RTI Connext DDS libraries', title: 'Download Packages'
                }
            }
        }

        stage('Build') {
            steps {
                sh 'python3 resources/ci_cd/linux_build.py'
            }
        }

        stage('Static Analysis') {
            steps {
                sh 'python3 resources/ci_cd/linux_static_analysis.py'
            }
        }
    }
}
