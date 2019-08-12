/**
 * Tests that a backtrace will appear in the $currentOp output if the backtrace option is
 * set to true and there is a latch timeout.
 *
 * @tags: [assumes_read_concern_unchanged, assumes_read_preference_unchanged]
 */
(function() {
"use strict";

const adminDB = db.getSiblingDB("admin");

const getCurrentOp = function(failpointOn) {
    var diagnostics = adminDB
        .aggregate(
            [
                {$currentOp: {idleConnections: true, backtrace: true}},
            ],
            {readConcern: {level: "local"}})
        .toArray();
    if (failpointOn == true) {
        for(var k = 0; k<diagnostics.length; k++) {
            if (diagnostics[k]["desc"] == "DiagnosticCaptureTest") {
                return diagnostics[k];
            }
        };
    }
    else {
        return diagnostics;
    }
};

assert.commandWorked(db.adminCommand(
    {"configureFailPoint": 'keepDiagnosticCaptureOnFailedLock', "mode": 'alwaysOn'}));
var result = getCurrentOp(true);

assert(result.hasOwnProperty("waitingForLatch"));
assert(result["waitingForLatch"].hasOwnProperty("timestamp"));
assert(result["waitingForLatch"].hasOwnProperty("captureName"));
assert(result["waitingForLatch"].hasOwnProperty("backtrace"));
result["waitingForLatch"]["backtrace"].forEach(function(obj) {
    assert(obj.hasOwnProperty("addr"));
    assert(typeof obj["addr"] === "string");
    assert(obj.hasOwnProperty("path"));
    assert(typeof obj["path"] === "string");
});

assert.commandWorked(
    db.adminCommand({"configureFailPoint": 'keepDiagnosticCaptureOnFailedLock', "mode": 'off'}));

result = getCurrentOp(false);
result.forEach(function(elem) {
    assert(!result.hasOwnProperty("waitingForLatch"));
})

})();
