def DETAILS_URL="https://www.rti.com"
def detailsText

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
                sh 'python3 resources/ci_cd/jenkins_output.py'

                script {
                    detailsText = readFile("jenkins_output.md")
                }

                publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                    status: 'IN_PROGRESS', title: 'Downloading', text: detailsText,
                    summary: ':arrow_down: Downloading RTI Connext DDS libraries...'

                sh 'python3 resources/ci_cd/linux_install.py'
                sh 'python3 resources/ci_cd/jenkins_output.py'

                script {
                    detailsText = readFile("jenkins_output.md")
                }
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: ':white_check_mark: RTI Connext DDS libraries downloaded.',
                        title: 'Passed', text: detailsText
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, title: 'Failed', text: detailsText,
                        summary: ':warning: Failed downloading RTI Connext DDS libraries.'
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, title: 'Aborted', text: detailsText,
                        summary: ':no_entry: The download of RTI Connext DDS libraries was aborted.'
                }
            }
        }

        stage('Build') {
            steps {
                publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME, 
                    status: 'IN_PROGRESS', summary: ':wrench: Building all the examples...', 
                    title: 'Building', text: detailsText

                sh 'python3 resources/ci_cd/linux_build.py'
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: ':white_check_mark: All the examples were built succesfully.',
                        title: 'Passed', text: detailsText
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, title: 'Failed', text: detailsText,
                        summary: ':warning: There was an error building the examples.'
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, title: 'Aborted', text: detailsText,
                        summary: ':no_entry: The examples build was aborted'
                }
            }
        }

        stage('Static Analysis') {
            steps {
                publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME, 
                    status: 'IN_PROGRESS', title: 'In progress', text: detailsText,
                    summary: ':mag: Analysing all the examples...'

                sh 'python3 resources/ci_cd/linux_static_analysis.py'
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: ':white_check_mark: Succesfully analysed',
                        title: 'Passed', text: detailsText
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, title: 'Failed', text: detailsText,
                        summary: ':warning: The static analysis failed'
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, title: 'Aborted', text: detailsText,
                        summary: ':no_entry: The static analysis was aborted'
                }
            }
        }
    }
}