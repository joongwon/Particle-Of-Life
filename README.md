# Particle Of Life

## 개요
### Particle of Life란?
Cellular Automata(세포자동자)를 변형한 것으로, 세포가 격자에 갇혀있는 대신 자유로이 떠다니며 서로 상호작용하며,
새로 태어나거나 죽지 않는다. 작용 반작용 법칙이 성립하지 않으며, 따라서 에너지 보존의 법칙도 성립하지 않는다.
단 마찰력으로 지나치게 빨라지는 것을 막는다.

### ParticleOfLife 프로그램
처음 실행하면 무작위로 입자 간의 상호작용을 결정한 후, 무작위로 입자를 생성하고 상호작용시킨다.
입자 간의 상호작용을 새로 생성하거나, 기존의 상호작용을 따르는 입자들을 새로 생성할 수 있다.
입자 간 상호작용을 저장하거나 불러올 수도 있다.

## 설정
config.txt에서 값을 수정할 수 있다. 이름: 설명
 * effective_radius: 기본 반발력의 범위
 * friction: 마찰의 크기를 결정하는 인자. a = friction일 때, v(t') = v(t) * e^(-a*dt)
 * maximum_fps: 최대 프레임율
 * maximum_interaction_distance: 상호작용의 최대 범위
 * maximum_interaction_strength: 상호작용의 세기를 결정하는 인자의 최대 크기 (값이 클수록 강하다)
 * minimum_fps: 최소 프레임율
 * particle_count: 입자의 갯수
 * region_size: 상호작용 검사를 위한 구역의 가로 세로 크기
 * regions_x_count: 구역의 가로 개수 (창 너비 = region_size * regions_x_count)
 * regions_y_count: 구역의 세로 개수 (창 높이 = region_size * regions_x_count)
 * repulsion_strength: 기본 반발력의 크기를 결정하는 인자 (값이 클수록 반발력이 강하다)
 * stablized_zone: 기본 반발력 또는 상호작용이 없는 중간 구역의 크기
 * type_count: 입자 유형의 가짓수
 * visible_radius: 입자의 크기

Copyright(c) 2021. Joongwon Ahn. All rights reserved.
