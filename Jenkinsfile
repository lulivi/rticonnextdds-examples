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
                publishChecks detailsURL: 'https://www.rti.com', name: 'Download Packages',
                    status: 'IN_PROGRESS', summary: 'Download RTI Connext DDS libraries',
                    title: 'Download Packages'

                sh 'python3 resources/ci_cd/linux_install.py'
            }

            post {
                success {
                    publishChecks detailsURL: 'https://www.rti.com', name: 'Download Packages',
                        summary: 'Downloads RTI Connext DDS libraries', title: 'Download Packages'
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: 'https://www.rti.com',
                        name: 'Download Packages', summary: 'Downloads RTI Connext DDS libraries',
                        title: 'Download Packages'
                }
            }
        }

        stage('Build') {
            steps {
                publishChecks detailsURL: 'https://www.rti.com', name: 'Build', 
                    status: 'IN_PROGRESS', summary: 'Build all the examples', title: 'Build'

                sh 'python3 resources/ci_cd/linux_build.py'
            }

            post {
                success {
                    publishChecks detailsURL: 'https://www.rti.com', name: 'Build',
                        summary: 'Build all the examples', title: 'Build'
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: 'https://www.rti.com', name: 'Build',
                        summary: 'Build all the examples', title: 'Build'
                }
            }
        }

        stage('Static Analysis') {
            steps {
                publishChecks detailsURL: 'https://www.rti.com', name: 'Static Analysis', 
                    status: 'IN_PROGRESS', summary: 'Analyses for all the examples',
                    title: 'Static Analysis'

                sh 'python3 resources/ci_cd/linux_static_analysis.py'
            }

            post {
                success {
                    publishChecks detailsURL: 'https://www.rti.com', name: 'Static Analysis',
                        summary: 'Analyses for all the examples', title: 'Static Analysis'
                }
                failure {
                    publishChecks conclusion: 'FAILURE', detailsURL: 'https://www.rti.com',
                        name: 'Static Analysis', summary: 'Analyses for all the examples',
                        title: 'Static Analysis'
                }
            }
        }
    }
}
