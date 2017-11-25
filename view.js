let bcw = 0;
let bgcolor = "hsl(50, 7%, 97%)";
let blockclocks = [
    6, 9, 3, 12, 12
];
let blockwidth = 22;
let blue = "hsl(200, 45%, 70%)";
let boarddims = { row: 38, col: 25 };
let board;
let canvas = document.getElementById('canvas');
const clockDirections = [
    12, 1, 3, 5, 6, 7, 9, 11
];
let colorme = "colorme";
let ctx = canvas.getContext('2d');
let gcw = 0;
let gravityTimeout = 1250;
let gravityTimer = gravityTimeout;
let gray1 = "hsl(280, 0%, 70%)";
let gray2 = "hsl(281, 1%, 70%)";
let green = "hsl(80, 45%, 70%)";
let highlight = "hsl(250, 100%, 80%)";
let ocw = 0;
let orange = "hsl(30, 45%, 70%)";
let rcw = 0;
let red = "hsl(0, 45%, 70%)";
let sd = 0;
let sl = 0;
let sr = 0;
let su = 0;
let updateInterval = 150;
let wholecoord = { row: 0, col: 0 };
let whole = [
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, colorme, colorme, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, colorme, colorme, colorme, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, colorme, colorme, colorme, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor],
    [bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor]
];
let vcw = 0;
let violet = "hsl(280, 45%, 70%)";
let ycw = 0;
let yellow = "hsl(50, 45%, 70%)";
let blockcolors = [red, orange, yellow, green, blue, violet, gray1, gray2];
let blockcolorsset = new Set(blockcolors);
function applyWhole() {
    for (let i = 0; i < whole.length; ++i) {
        for (let j = 0; j < whole[0].length; ++j) {
            if (whole[i][j] !== bgcolor)
                board[wholecoord.row + i][wholecoord.col + j] = true;
        }
    }
    wholecoord.row = 0;
    wholecoord.col = 0;
}
function bounds(target) {
    let minx = 999;
    let maxx = -1;
    let miny = 999;
    let maxy = -1;
    for (let i = 0; i < target.length; ++i) {
        for (let j = 0; j < target[0].length; ++j) {
            if (target[i][j] !== bgcolor) {
                if (j < minx)
                    minx = j;
                if (j > maxx)
                    maxx = j;
                if (i < miny)
                    miny = i;
                if (i > maxy)
                    maxy = i;
            }
        }
    }
    return { left: minx, right: maxx, top: miny, bottom: maxy };
}
function centerWhole(target) {
    let b = bounds(target);
    return { row: Math.floor((b.bottom - b.top) / 2 + b.top), col: Math.floor((b.right - b.left) / 2 + b.left) };
}
function clearBoard() {
    let ret = new Array();
    for (let i = 0; i < boarddims.row; ++i) {
        let row = new Array();
        for (let j = 0; j < boarddims.col; ++j) {
            row.push(false);
        }
        ret.push(row);
    }
    return ret;
}
function clockMove(color, cwTrueCcwFalse) {
    let curCoords = getCoords(color);
    let newCoords = { row: curCoords.row, col: curCoords.col };
    let adjCoords = getAdjOccupied(curCoords);
    if (adjCoords.length < 4) {
        for (let i = 0; i < adjCoords.length; ++i) {
            let clockDirection = getClockDirection(adjCoords[i], curCoords);
            let cw = clockwise(cwTrueCcwFalse, clockDirection);
            let cw2 = clockwise(cwTrueCcwFalse, cw);
            let step1 = getPotentialCoords(adjCoords[i], cw);
            let step1Whole = cloneWhole(whole);
            step1Whole[curCoords.row][curCoords.col] = bgcolor;
            let potentialCoords = getPotentialCoords(adjCoords[i], cw2);
            let potentialWhole = cloneWhole(whole);
            potentialWhole[curCoords.row][curCoords.col] = bgcolor;
            potentialWhole[potentialCoords.row][potentialCoords.col] = color;
            // TODO step1Whole should be contiguous except for the piece in motion.
            if (isOpen(step1) && isOpen(potentialCoords) && contiguous(potentialCoords, potentialWhole) === true)
                newCoords = potentialCoords;
            else if (isOpen(potentialCoords) === false || contiguous(potentialCoords, potentialWhole) === false) {
                let clockDirectionDiag = getClockDirection(potentialCoords, curCoords);
                let cwDiag = clockwise(cwTrueCcwFalse, clockDirectionDiag);
                let potentialCoordsDiag = getPotentialCoords(potentialCoords, cwDiag);
                let potentialWholeDiag = cloneWhole(whole);
                potentialWholeDiag[curCoords.row][curCoords.col] = bgcolor;
                potentialWholeDiag[potentialCoordsDiag.row][potentialCoordsDiag.col] = color;
                if (isOpen(potentialCoordsDiag) && contiguous(potentialCoordsDiag, potentialWholeDiag) === true)
                    newCoords = potentialCoordsDiag;
            }
        }
    }
    whole[curCoords.row][curCoords.col] = bgcolor;
    whole[newCoords.row][newCoords.col] = color;
}
function clockwise(cw, clockDirection) {
    for (let i = 0; i < clockDirections.length; ++i) {
        if (clockDirections[i] === clockDirection) {
            if (cw === true)
                return clockDirections[(i + 1) % clockDirections.length];
            else {
                if ((i - 1) === -1)
                    return clockDirections[clockDirections.length - 1];
                else
                    return clockDirections[i - 1];
            }
        }
    }
    return undefined;
}
function cloneWhole(target) {
    let ret = new Array();
    for (let i = 0; i < target.length; ++i) {
        ret.push(target[i].slice());
    }
    return ret;
}
function colorWhole() {
    let colorindex = 0;
    let r1 = randomColorMe(whole);
    let r2 = randomColorMe(whole);
    whole[r1.row][r1.col] = gray1;
    whole[r2.row][r2.col] = gray2;
    for (let i = 0; i < whole.length; ++i) {
        for (let j = 0; j < whole[0].length; ++j) {
            if (whole[i][j] === colorme) {
                whole[i][j] = blockcolors[colorindex++];
            }
        }
    }
}
function contiguous(startingCoord, potential) {
    let l = new Array();
    let s = new Set();
    l.push(startingCoord);
    do {
        let m = l.pop();
        if (setHasEquality(s, m) === false)
            s.add(m);
        let y = m.row;
        let x = m.col;
        if (y - 1 > 0 && potential[y - 1][x] != bgcolor && setHasEquality(s, { row: y - 1, col: x }) === false)
            l.push({ row: y - 1, col: x });
        if (y + 1 < potential.length && potential[y + 1][x] != bgcolor && setHasEquality(s, { row: y + 1, col: x }) === false)
            l.push({ row: y + 1, col: x });
        if (x - 1 > 0 && potential[y][x - 1] != bgcolor && setHasEquality(s, { row: y, col: x - 1 }) === false)
            l.push({ row: y, col: x - 1 });
        if (x + 1 < potential[0].length && potential[y][x + 1] != bgcolor && setHasEquality(s, { row: y, col: x + 1 }) === false)
            l.push({ row: y, col: x + 1 });
    } while (l.length > 0);
    return s.size == blockcolors.length;
}
function getAdjOccupied(coords) {
    let ret = new Array();
    if (isOpen({ row: coords.row - 1, col: coords.col }) === false)
        ret.push({ row: coords.row - 1, col: coords.col });
    if (isOpen({ row: coords.row + 1, col: coords.col }) === false)
        ret.push({ row: coords.row + 1, col: coords.col });
    if (isOpen({ row: coords.row, col: coords.col - 1 }) === false)
        ret.push({ row: coords.row, col: coords.col - 1 });
    if (isOpen({ row: coords.row, col: coords.col + 1 }) === false)
        ret.push({ row: coords.row, col: coords.col + 1 });
    return ret;
}
function getClockDirection(center, d) {
    // Assumes center and d are adj.
    if (d.row === center.row - 1) {
        if (d.col === center.col - 1)
            return 11;
        if (d.col === center.col)
            return 12;
        if (d.col === center.col + 1)
            return 1;
    }
    if (d.row === center.row) {
        if (d.col === center.col - 1)
            return 9;
        if (d.col === center.col)
            return 0;
        if (d.col === center.col + 1)
            return 3;
    }
    if (d.row === center.row + 1) {
        if (d.col === center.col - 1)
            return 7;
        if (d.col === center.col)
            return 6;
        if (d.col === center.col + 1)
            return 5;
    }
    return undefined;
}
function getCoords(color) {
    return getCoordsPotentialWhole(color, whole);
}
function getCoordsPotentialWhole(color, potentialWhole) {
    for (let i = 0; i < potentialWhole.length; ++i) {
        for (let j = 0; j < potentialWhole[0].length; ++j) {
            if (potentialWhole[i][j] === color)
                return { row: i, col: j };
        }
    }
    return { row: -1, col: -1 };
}
function getPotentialCoords(coords, clockDirection) {
    let y = coords.row;
    let x = coords.col;
    if (clockDirection === 12)
        y -= 1;
    if (clockDirection === 1) {
        y -= 1;
        x += 1;
    }
    if (clockDirection === 3)
        x += 1;
    if (clockDirection === 5) {
        y += 1;
        x += 1;
    }
    if (clockDirection === 6)
        y += 1;
    if (clockDirection === 7) {
        y += 1;
        x -= 1;
    }
    if (clockDirection === 9)
        x -= 1;
    if (clockDirection === 11) {
        y -= 1;
        x -= 1;
    }
    if (y < 0 || y >= whole.length)
        y = undefined;
    if (x < 0 || x >= whole[0].length)
        x = undefined;
    return { row: y, col: x };
}
function isOpen(coords) {
    // TODO project move onto board and see if board location is open also.
    // And in board bounds.
    if (coords.col + wholecoord.col < 0)
        return false;
    if (coords.col + wholecoord.col >= boarddims.col)
        return false;
    if (coords.row + wholecoord.row >= boarddims.row)
        return false;
    if (board[coords.row + wholecoord.row][coords.col + wholecoord.col])
        return false;
    if (whole[coords.row])
        return whole[coords.row][coords.col] === bgcolor;
    return false;
}
function kd(e) {
    if (e.defaultPrevented)
        return;
    if (e.code === "Digit1") {
        rcw = 1;
    }
    if (e.code === "KeyQ") {
        rcw = -1;
    }
    if (e.code === "Digit2") {
        ocw = 1;
    }
    if (e.code === "KeyW") {
        ocw = -1;
    }
    if (e.code === "Digit3") {
        ycw = 1;
    }
    if (e.code === "KeyE") {
        ycw = -1;
    }
    if (e.code === "Digit4") {
        gcw = 1;
    }
    if (e.code === "KeyR") {
        gcw = -1;
    }
    if (e.code === "Digit5") {
        bcw = 1;
    }
    if (e.code === "KeyT") {
        bcw = -1;
    }
    if (e.code === "Digit6") {
        vcw = 1;
    }
    if (e.code === "KeyY") {
        vcw = -1;
    }
    if (e.code === "ArrowRight") {
        sr = 1;
    }
    if (e.code === "ArrowLeft") {
        sl = 1;
    }
    if (e.code === "ArrowDown") {
        sd = 1;
    }
    if (e.code === "ArrowUp") {
        su = 1;
    }
    //e.preventDefault(); todo put this in in release
}
function ku(e) {
    if (e.defaultPrevented)
        return;
    if (e.code === "Digit1") {
        rcw = 0;
    }
    if (e.code === "KeyQ") {
        rcw = 0;
    }
    if (e.code === "Digit2") {
        ocw = 0;
    }
    if (e.code === "KeyW") {
        ocw = 0;
    }
    if (e.code === "Digit3") {
        ycw = 0;
    }
    if (e.code === "KeyE") {
        ycw = 0;
    }
    if (e.code === "Digit4") {
        gcw = 0;
    }
    if (e.code === "KeyR") {
        gcw = 0;
    }
    if (e.code === "Digit5") {
        bcw = 0;
    }
    if (e.code === "KeyT") {
        bcw = 0;
    }
    if (e.code === "Digit6") {
        vcw = 0;
    }
    if (e.code === "KeyY") {
        vcw = 0;
    }
    if (e.code === "ArrowRight") {
        sr = 0;
    }
    if (e.code === "ArrowLeft") {
        sl = 0;
    }
    if (e.code === "ArrowDown") {
        sd = 0;
    }
    if (e.code === "ArrowUp") {
        su = 0;
    }
    //e.preventDefault(); todo put this in in release
}
function proc() {
    if (rcw === 1) {
        clockMove(red, true);
    }
    else if (rcw === -1) {
        clockMove(red, false);
    }
    if (ocw === 1) {
        clockMove(orange, true);
    }
    else if (ocw === -1) {
        clockMove(orange, false);
    }
    if (ycw === 1) {
        clockMove(yellow, true);
    }
    else if (ycw === -1) {
        clockMove(yellow, false);
    }
    if (gcw === 1) {
        clockMove(green, true);
    }
    else if (gcw === -1) {
        clockMove(green, false);
    }
    if (bcw === 1) {
        clockMove(blue, true);
    }
    else if (bcw === -1) {
        clockMove(blue, false);
    }
    if (vcw === 1) {
        clockMove(violet, true);
    }
    else if (vcw === -1) {
        clockMove(violet, false);
    }
    let center = centerWhole(whole);
    if (center.row < (whole.length / 2))
        shiftWhole(whole, 6);
    else if (center.row > (whole.length / 2))
        shiftWhole(whole, 12);
    if (center.col < (whole[0].length / 2))
        shiftWhole(whole, 3);
    else if (center.col > (whole.length / 2))
        shiftWhole(whole, 9);
    if (sr === 1) {
        ++wholecoord.col;
        for (let i = 0; i < whole.length; ++i) {
            for (let j = 0; j < whole[0].length; ++j) {
                if (whole[i][j] !== bgcolor) {
                    let color = whole[i][j];
                    whole[i][j] = bgcolor;
                    if (isOpen({ row: i, col: j }) === false)
                        --wholecoord.col;
                    whole[i][j] = color;
                }
            }
        }
    }
    if (sl === 1) {
        --wholecoord.col;
        for (let i = 0; i < whole.length; ++i) {
            for (let j = 0; j < whole[0].length; ++j) {
                if (whole[i][j] !== bgcolor) {
                    let color = whole[i][j];
                    whole[i][j] = bgcolor;
                    if (isOpen({ row: i, col: j }) === false)
                        ++wholecoord.col;
                    whole[i][j] = color;
                }
            }
        }
    }
    let apply = false;
    if (su === 1) {
        gravityTimer = updateInterval;
        let hit = false;
        while (hit === false) {
            ++wholecoord.row;
            for (let i = 0; i < whole.length; ++i) {
                for (let j = 0; j < whole[0].length; ++j) {
                    if (whole[i][j] !== bgcolor) {
                        let color = whole[i][j];
                        whole[i][j] = bgcolor;
                        if (isOpen({ row: i, col: j }) === false) {
                            --wholecoord.row;
                            hit = true;
                            apply = true;
                        }
                        whole[i][j] = color;
                    }
                }
            }
        }
    }
    else if (sd === 1) {
        gravityTimer = updateInterval;
        ++wholecoord.row;
        for (let i = 0; i < whole.length; ++i) {
            for (let j = 0; j < whole[0].length; ++j) {
                if (whole[i][j] !== bgcolor) {
                    let color = whole[i][j];
                    whole[i][j] = bgcolor;
                    if (isOpen({ row: i, col: j }) === false) {
                        --wholecoord.row;
                        apply = true;
                    }
                    whole[i][j] = color;
                }
            }
        }
    }
    else {
        gravityTimer -= updateInterval;
        if (gravityTimer < 0) {
            gravityTimer += gravityTimeout;
            ++wholecoord.row;
            for (let i = 0; i < whole.length; ++i) {
                for (let j = 0; j < whole[0].length; ++j) {
                    if (whole[i][j] !== bgcolor) {
                        let color = whole[i][j];
                        whole[i][j] = bgcolor;
                        if (isOpen({ row: i, col: j }) === false) {
                            --wholecoord.row;
                            apply = true;
                        }
                        whole[i][j] = color;
                    }
                }
            }
        }
    }
    if (apply === true) {
        applyWhole();
    }
}
function randomColorMe(potentialWhole) {
    let colorMes = new Array();
    for (let i = 0; i < potentialWhole.length; ++i) {
        for (let j = 0; j < potentialWhole[0].length; ++j) {
            if (potentialWhole[i][j] === colorme) {
                colorMes.push({ row: i, col: j });
            }
        }
    }
    return colorMes[randomRange(0, colorMes.length - 1)];
}
function renderBoard() {
    ctx.fillStyle = bgcolor;
    ctx.fillRect(0, 0, blockwidth * boarddims.col, blockwidth * boarddims.row);
    for (let i = 0; i < boarddims.row; ++i) {
        for (let j = 0; j < boarddims.col; ++j) {
        }
    }
    for (let i = 0; i < whole.length; ++i) {
        for (let j = 0; j < whole[0].length; ++j) {
            if (blockcolorsset.has(whole[i][j])) {
                ctx.fillStyle = whole[i][j];
                ctx.fillRect((wholecoord.col + j) * blockwidth, (wholecoord.row + i) * blockwidth, blockwidth - 1, blockwidth - 1);
                ctx.fillStyle = highlight;
                let c = 0;
                for (let k = 0; k < blockcolors.length; ++k) {
                    if (blockcolors[k] === whole[i][j])
                        c = k;
                }
                if (blockclocks[c] === 12)
                    ctx.fillRect((wholecoord.col + j) * blockwidth, (wholecoord.row + i) * blockwidth, blockwidth - 1, 1);
                if (blockclocks[c] === 3)
                    ctx.fillRect((wholecoord.col + j + 1) * blockwidth - 2, (wholecoord.row + i) * blockwidth, 1, blockwidth - 1);
                if (blockclocks[c] === 6)
                    ctx.fillRect((wholecoord.col + j) * blockwidth, (wholecoord.row + i + 1) * blockwidth - 1, blockwidth - 1, 1);
                if (blockclocks[c] === 9)
                    ctx.fillRect((wholecoord.col + j) * blockwidth, (wholecoord.row + i) * blockwidth, 1, blockwidth - 1);
            }
        }
    }
    for (let i = 0; i < boarddims.row; ++i) {
        for (let j = 0; j < boarddims.col; ++j) {
            if (board[i][j]) {
                ctx.fillStyle = gray1;
                ctx.fillRect(j * blockwidth, i * blockwidth, blockwidth - 1, blockwidth - 1);
            }
        }
    }
}
function randomRange(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min + 1)) + min;
}
function setHasEquality(s, hasThis) {
    for (let entry of s) {
        if (entry.row === hasThis.row && entry.col === hasThis.col)
            return true;
    }
    return false;
}
function shiftWhole(target, clockDirection) {
    if (clockDirection === 12) {
        for (let i = 0; i < target.length; ++i) {
            for (let j = 0; j < target[0].length; ++j) {
                if (i + 1 < target.length) {
                    target[i][j] = target[i + 1][j];
                }
                else {
                    target[i][j] = bgcolor;
                }
            }
        }
        wholecoord.row += 1;
    }
    if (clockDirection === 6) {
        for (let i = target.length - 1; i >= 0; --i) {
            for (let j = 0; j < target[0].length; ++j) {
                if (i - 1 >= 0) {
                    target[i][j] = target[i - 1][j];
                }
                else {
                    target[i][j] = bgcolor;
                }
            }
        }
        wholecoord.row -= 1;
    }
    if (clockDirection === 3) {
        for (let i = 0; i < target.length; ++i) {
            for (let j = target[0].length - 1; j >= 0; --j) {
                if (j - 1 >= 0) {
                    target[i][j] = target[i][j - 1];
                }
                else {
                    target[i][j] = bgcolor;
                }
            }
        }
        wholecoord.col -= 1;
    }
    if (clockDirection === 9) {
        for (let i = 0; i < target.length; ++i) {
            for (let j = 0; j < target[0].length; ++j) {
                if (j + 1 < target[0].length) {
                    target[i][j] = target[i][j + 1];
                }
                else {
                    target[i][j] = bgcolor;
                }
            }
        }
        wholecoord.col += 1;
    }
}
function update() {
    proc();
    renderBoard();
}
// Main
board = clearBoard();
colorWhole();
window.addEventListener("keydown", kd, true);
window.addEventListener("keyup", ku, true);
setInterval(() => update(), updateInterval);
