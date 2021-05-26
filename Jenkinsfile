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
                    status: 'IN_PROGRESS', summary: 'Download RTI Connext DDS libraries',
                    title: 'In progress', text: detailsText

                sh 'python3 resources/ci_cd/linux_install.py'
                sh 'python3 resources/ci_cd/jenkins_output.py'

                script {
                    detailsText = readFile("jenkins_output.md")
                }
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: 'Downloads RTI Connext DDS libraries',
                        title: 'Passed', text: detailsText
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Downloads RTI Connext DDS libraries',
                        title: 'Failed', text: detailsText
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Downloads RTI Connext DDS libraries',
                        title: 'Aborted', text: detailsText
                }
            }
        }

        stage('Build') {
            steps {
                publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME, 
                    status: 'IN_PROGRESS', summary: 'Build all the examples', 
                    title: 'In progress', text: detailsText

                sh 'python3 resources/ci_cd/linux_build.py'
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: 'Build all the examples', title: 'Passed',
                        text: detailsText
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Build all the examples',
                        title: 'Failed', text: detailsText
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Build all the examples',
                        title: 'Aborted', text: detailsText
                }
            }
        }

        stage('Static Analysis') {
            steps {
                publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME, 
                    status: 'IN_PROGRESS', summary: 'Analyses all the examples',
                    title: 'In progress', text: detailsText

                sh 'python3 resources/ci_cd/linux_static_analysis.py'
            }

            post {
                success {
                    publishChecks detailsURL: DETAILS_URL, name: STAGE_NAME,
                        summary: 'Analyses all the examples', title: 'Passed',
                        text: detailsText
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Analyses all the examples',
                        title: 'Failed', text: detailsText
                }
                aborted {
                    publishChecks conclusion: 'CANCELED', detailsURL: DETAILS_URL,
                        name: STAGE_NAME, summary: 'Analyses all the examples',
                        title: 'Aborted', text: detailsText
                }
            }
        }
    }
}
