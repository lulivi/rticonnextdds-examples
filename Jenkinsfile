def DETAILS_URL="https://www.rti.com"

pipeline {
    agent {
        dockerfile true
    }

    environment {
        RTI_MIN_PACKAGE_URL = credentials('rti-min-package-url')
        RTI_INSTALLATION_PATH = "${WORKSPACE}"
    }

    stages {
        stage('Download Packages') {
            steps {
                publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                    status: 'IN_PROGRESS', summary: 'Download RTI Connext DDS libraries',
                    title: 'Download Packages'

                sh 'python3 resources/ci_cd/linux_install.py'
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: 'Downloads RTI Connext DDS libraries', title: 'Download Packages'
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Downloads RTI Connext DDS libraries',
                        title: 'Download Packages'
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Analyses all the examples',
                        title: 'Static Analysis'
                }
            }
        }

        stage('Build') {
            steps {
                publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME, 
                    status: 'IN_PROGRESS', summary: 'Build all the examples', title: 'Build'

                sh 'python3 resources/ci_cd/linux_build.py'
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: 'Build all the examples', title: 'Build'
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: 'Build all the examples', title: 'Build'
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Analyses all the examples',
                        title: 'Static Analysis'
                }
            }
        }

        stage('Static Analysis') {
            steps {
                publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME, 
                    status: 'IN_PROGRESS', summary: 'Analyses all the examples',
                    title: 'Static Analysis'

                sh 'python3 resources/ci_cd/linux_static_analysis.py'
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: 'Analyses all the examples', title: 'Static Analysis'
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Analyses all the examples',
                        title: 'Static Analysis'
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Analyses all the examples',
                        title: 'Static Analysis'
                }
            }
        }
    }
}
