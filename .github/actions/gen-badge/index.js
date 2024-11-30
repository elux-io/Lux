const core = require('@actions/core')
const github = require('@actions/github')

try {
	const badgeName = core.getInput('badge-name')
	const status = core.getInput('status')

	if (status === 'failure') {

	}

	// github.token
	const context = github.context;


	core.info(`badge name: ${badgeName}, status: ${status}`)
} catch (error) {
	core.setFailed(error.message)
}
