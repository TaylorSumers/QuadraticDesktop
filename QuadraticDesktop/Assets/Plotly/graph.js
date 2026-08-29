const config = {
    responsive: true,
    displaylogo: false,
    scrollZoom: true
};

const emptyLayout = {
    margin: {
        left: 0,
        right: 0,
        top: 0,
        bottom: 0
    },

    showlegend: false,

    xaxis: {
        title: "x",
        tickmode: "linear",
        tick0: 0,
        dtick: 1,
        gridcolor: "#e5e7eb",
        zerolinecolor: "#111827"
    },

    yaxis: {
        title: "y",
        tickmode: "linear",
        tick0: 0,
        dtick: 1,
        scaleanchor: "x",
        scaleratio: 1,
        gridcolor: "#e5e7eb",
        zerolinecolor: "#111827"
    }
};

function drawQuadratic(a, b, c, x1, x2) {
    const xValues = [];
    const yValues = [];

    const xMin = -10;
    const xMax = 10;
    const pointCount = 100;

    for (let i = 0; i <= pointCount; ++i) {
        const x = xMin + (xMax - xMin) * i / pointCount;
        const y = a * x * x + b * x + c;

        xValues.push(x);
        yValues.push(y);
    }

    const trace = {
        x: xValues,
        y: yValues,
        type: "scatter",
        mode: "lines",

        line: {
            color: "#ef4444",
            width: 3
        },

        hovertemplate:
            "x = %{x:.3f}<br>" +
            "y = %{y:.3f}" +
            "<extra></extra>"
    };

    //
    const traces = [trace];

    const EPS = 1e-6;

    // При a ≈ 0 функция линейная и вершины параболы нет
    if (Math.abs(a) > EPS) {
        const vertexX = -b / (2 * a);
        const vertexY =
            a * vertexX * vertexX +
            b * vertexX +
            c;

        const vertexTrace = {
            x: [vertexX],
            y: [vertexY],

            type: "scatter",
            mode: "markers",

            marker: {
                color: "MediumPurple",
                size: 12,
                symbol: "circle",
                line: {
                    color: "#ffffff",
                    width: 2
                }
            },

            hovertemplate:
                "<b>Вершина</b><br>" +
                "x = %{x:.3f}<br>" +
                "y = %{y:.3f}" +
                "<extra></extra>"
        };

        //
        const rootTrace1 = {
            x: [x1],
            y: [0],

            type: "scatter",
            mode: "markers",

            marker: {
                color: "red",
                size: 12,
                symbol: "circle",
                line: {
                    color: "#ffffff",
                    width: 2
                }
            },

            hovertemplate:
                "<b>x₁</b><br>" +
                "x = %{x:.3f}<br>" +
                "y = %{y:.3f}" +
                "<extra></extra>"
        }

        const rootTrace2 = {
            x: [x2],
            y: [0],

            type: "scatter",
            mode: "markers",

            marker: {
                color: "red",
                size: 12,
                symbol: "circle",
                line: {
                    color: "#ffffff",
                    width: 2
                }
            },

            hovertemplate:
                "<b>x₂</b><br>" +
                "x = %{x:.3f}<br>" +
                "y = %{y:.3f}" +
                "<extra></extra>"
        }
        //

        traces.push(vertexTrace);

        //
        traces.push(rootTrace1);
        traces.push(rootTrace2);
        //
    }
    //

    const layout = {
        margin: {
            left: 0,
            right: 0,
            top: 0,
            bottom: 0
        },

        showlegend: false,

        xaxis: {
            title: "x",
            range: [xMin, xMax],
            tickmode: "linear",
            tick0: 0,
            dtick: 1,
            gridcolor: "#e5e7eb",
            zerolinecolor: "#111827"
        },

        yaxis: {
            title: "y",
            autorange: false,
            tickmode: "linear",
            tick0: 0,
            dtick: 1,
            scaleanchor: "x",
            scaleratio: 1,
            gridcolor: "#e5e7eb",
            zerolinecolor: "#111827"
        }
    };

    Plotly.react("graph", traces, layout, config);
}

Plotly.newPlot("graph", [], emptyLayout, config);

window.chrome.webview.addEventListener("message", event => {
    const { a, b, c, x1, x2 } = event.data;

    drawQuadratic(a, b, c, x1, x2);
});