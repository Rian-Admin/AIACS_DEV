import React, { useRef, useEffect } from 'react';
import PropTypes from 'prop-types';
import * as d3 from 'd3';

/**
 * D3.js를 사용한 라인 차트 컴포넌트
 * @param {Object} props
 * @param {Array} props.data - 차트 데이터
 * @param {string} props.xKey - X축 데이터 키
 * @param {Array} props.yKeys - Y축 데이터 키(들) (다중 라인을 위한 배열)
 * @param {Array} props.colors - 각 라인의 색상 배열
 * @param {Array} props.labels - 각 라인의 레이블 배열
 * @param {string} props.xLabel - X축 레이블
 * @param {string} props.yLabel - Y축 레이블
 * @param {number} props.height - 차트 높이
 * @param {string} props.backgroundColor - 배경 색상
 * @param {string} props.textColor - 텍스트 색상
 * @param {string} props.gridColor - 그리드 색상
 * @returns {JSX.Element}
 */
const LineChartD3 = ({ 
  data, 
  xKey, 
  yKeys, 
  colors = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042'], 
  labels = [],
  xLabel, 
  yLabel, 
  height = 300,
  backgroundColor = '#0c1e36',
  textColor = '#ffffff',
  gridColor = '#1e3a5a'
}) => {
  const svgRef = useRef(null);
  
  useEffect(() => {
    if (!data || data.length === 0 || !yKeys || yKeys.length === 0) return;
    
    // SVG 컨테이너와 크기 정의
    const svg = d3.select(svgRef.current);
    svg.selectAll("*").remove(); // 이전 차트 제거
    
    const margin = { top: 30, right: 30, bottom: 50, left: 60 };
    const width = svgRef.current.clientWidth - margin.left - margin.right;
    const chartHeight = height - margin.top - margin.bottom;
    
    // 메인 그래프 영역 생성
    const chart = svg
      .append('g')
      .attr('transform', `translate(${margin.left}, ${margin.top})`);
    
    // 툴팁 생성
    const tooltip = d3.select('body')
      .append('div')
      .attr('class', 'd3-tooltip')
      .style('position', 'absolute')
      .style('visibility', 'hidden')
      .style('background-color', '#0a1929')
      .style('border', '1px solid #1e3a5a')
      .style('border-radius', '4px')
      .style('padding', '8px')
      .style('color', '#ffffff')
      .style('pointer-events', 'none')
      .style('z-index', 1000);
    
    // 색상 스케일 설정
    const color = d3.scaleOrdinal()
      .domain(yKeys)
      .range(colors);
    
    // X 스케일 설정
    const x = d3.scalePoint()
      .domain(data.map(d => d[xKey]))
      .range([0, width])
      .padding(0.1);
    
    // Y 스케일 설정
    const y = d3.scaleLinear()
      .domain([0, d3.max(data, d => Math.max(...yKeys.map(key => d[key] || 0))) * 1.1])
      .range([chartHeight, 0]);
    
    // X 축 생성
    chart.append('g')
      .attr('transform', `translate(0, ${chartHeight})`)
      .call(d3.axisBottom(x))
      .call(g => g.select('.domain').attr('stroke', textColor))
      .call(g => g.selectAll('.tick line').attr('stroke', textColor))
      .call(g => g.selectAll('.tick text').attr('fill', textColor))
      .selectAll('text')
      .style('text-anchor', 'end')
      .attr('transform', 'rotate(-45)')
      .attr('dx', '-.8em')
      .attr('dy', '.15em');
    
    // Y 축 생성
    chart.append('g')
      .call(d3.axisLeft(y))
      .call(g => g.select('.domain').attr('stroke', textColor))
      .call(g => g.selectAll('.tick line').attr('stroke', textColor))
      .call(g => g.selectAll('.tick text').attr('fill', textColor));
    
    // 그리드 라인 추가
    chart.append('g')
      .attr('class', 'grid')
      .call(d3.axisLeft(y)
        .tickSize(-width)
        .tickFormat(''))
      .call(g => g.select('.domain').remove())
      .call(g => g.selectAll('.tick line')
        .attr('stroke', gridColor)
        .attr('stroke-dasharray', '3,3')
        .attr('stroke-opacity', 0.7));
    
    // 라인 생성기 정의
    const line = d3.line()
      .x(d => x(d[xKey]))
      .y(d => y(d.value))
      .curve(d3.curveMonotoneX);
    
    // 각 라인 그리기
    yKeys.forEach((yKey, i) => {
      // 필요한 형식으로 데이터 변환
      const lineData = data.map(d => ({
        [xKey]: d[xKey],
        value: d[yKey] || 0
      }));
      
      // 영역 그라데이션 (선택적)
      const areaGradient = chart.append('linearGradient')
        .attr('id', `area-gradient-${i}`)
        .attr('gradientUnits', 'userSpaceOnUse')
        .attr('x1', 0)
        .attr('y1', chartHeight)
        .attr('x2', 0)
        .attr('y2', 0);
        
      areaGradient.append('stop')
        .attr('offset', '0%')
        .attr('stop-color', color(yKey))
        .attr('stop-opacity', 0.05);
        
      areaGradient.append('stop')
        .attr('offset', '100%')
        .attr('stop-color', color(yKey))
        .attr('stop-opacity', 0.2);
      
      // 영역 추가
      const area = d3.area()
        .x(d => x(d[xKey]))
        .y0(chartHeight)
        .y1(d => y(d.value))
        .curve(d3.curveMonotoneX);
        
      chart.append('path')
        .datum(lineData)
        .attr('fill', `url(#area-gradient-${i})`)
        .attr('d', area)
        .attr('opacity', 0)
        .transition()
        .duration(800)
        .attr('opacity', 1);
      
      // 애니메이션을 위한 라인 패스 길이 계산
      const path = chart.append('path')
        .datum(lineData)
        .attr('fill', 'none')
        .attr('stroke', color(yKey))
        .attr('stroke-width', 2.5)
        .attr('stroke-linejoin', 'round')
        .attr('stroke-linecap', 'round')
        .attr('d', line);
        
      // 점선 효과 (선택적으로 적용)
      if (i % 2 === 1) {
        path.attr('stroke-dasharray', '5,5');
      }
      
      // 라인 애니메이션
      const pathLength = path.node().getTotalLength();
      
      path
        .attr('stroke-dasharray', pathLength)
        .attr('stroke-dashoffset', pathLength)
        .transition()
        .duration(1000)
        .ease(d3.easeLinear)
        .attr('stroke-dashoffset', 0);
      
      // 데이터 포인트 추가
      chart.selectAll(`.dot-group-${i}`)
        .data(lineData)
        .enter()
        .append('circle')
        .attr('class', `dot-group-${i}`)
        .attr('cx', d => x(d[xKey]))
        .attr('cy', d => y(d.value))
        .attr('r', 0) // 시작 크기
        .attr('fill', color(yKey))
        .attr('stroke', backgroundColor)
        .attr('stroke-width', 2)
        .style('opacity', 0.8)
        .style('cursor', 'pointer')
        .transition()
        .delay((d, j) => j * 50 + 500) // 라인 애니메이션 후 점 표시
        .duration(300)
        .attr('r', 5); // 최종 크기
      
      // 데이터 포인트 툴팁 상호작용
      chart.selectAll(`.dot-group-${i}`)
        .on('mouseover', function(event, d) {
          const label = labels[i] || yKey;
          
          d3.select(this)
            .transition()
            .duration(200)
            .attr('r', 7)
            .style('opacity', 1);
            
          tooltip
            .style('visibility', 'visible')
            .html(`
              <div style="font-weight: bold; margin-bottom: 4px;">${d[xKey]}</div>
              <div style="display: flex; align-items: center;">
                <div style="width: 10px; height: 10px; background-color: ${color(yKey)}; margin-right: 5px; border-radius: 50%;"></div>
                <span>${label}: ${d.value}</span>
              </div>
            `)
            .style('left', (event.pageX + 10) + 'px')
            .style('top', (event.pageY - 20) + 'px');
        })
        .on('mouseout', function() {
          d3.select(this)
            .transition()
            .duration(200)
            .attr('r', 5)
            .style('opacity', 0.8);
            
          tooltip.style('visibility', 'hidden');
        });
    });
    
    // X 축 레이블 추가
    chart.append('text')
      .attr('class', 'x-label')
      .attr('text-anchor', 'middle')
      .attr('x', width / 2)
      .attr('y', chartHeight + margin.bottom - 5)
      .attr('fill', textColor)
      .text(xLabel);
    
    // Y 축 레이블 추가
    chart.append('text')
      .attr('class', 'y-label')
      .attr('text-anchor', 'middle')
      .attr('transform', `translate(${-margin.left / 1.5}, ${chartHeight / 2}) rotate(-90)`)
      .attr('fill', textColor)
      .text(yLabel);
    
    // 범례 추가
    if (labels.length > 0) {
      const legend = chart.append('g')
        .attr('font-family', 'sans-serif')
        .attr('font-size', 10)
        .attr('text-anchor', 'end')
        .selectAll('g')
        .data(labels.length > 0 ? labels : yKeys)
        .enter().append('g')
        .attr('transform', (d, i) => `translate(0, ${i * 20})`);

      legend.append('rect')
        .attr('x', width - 19)
        .attr('width', 19)
        .attr('height', 19)
        .attr('fill', (d, i) => color(yKeys[i]));

      legend.append('text')
        .attr('x', width - 24)
        .attr('y', 9.5)
        .attr('dy', '0.32em')
        .attr('fill', textColor)
        .text(d => d);
    }
    
    // 정리 함수
    return () => {
      tooltip.remove();
    };
  }, [data, xKey, yKeys, colors, labels, xLabel, yLabel, height, backgroundColor, textColor, gridColor]);
  
  return (
    <div style={{ width: '100%', height: height, backgroundColor: backgroundColor }}>
      <svg ref={svgRef} width="100%" height={height}></svg>
    </div>
  );
};

LineChartD3.propTypes = {
  data: PropTypes.array.isRequired,
  xKey: PropTypes.string.isRequired,
  yKeys: PropTypes.array.isRequired,
  colors: PropTypes.array,
  labels: PropTypes.array,
  xLabel: PropTypes.string,
  yLabel: PropTypes.string,
  height: PropTypes.number,
  backgroundColor: PropTypes.string,
  textColor: PropTypes.string,
  gridColor: PropTypes.string
};

export default LineChartD3; 