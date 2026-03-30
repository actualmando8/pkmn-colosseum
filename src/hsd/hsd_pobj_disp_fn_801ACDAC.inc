    nofralloc
    stwu r1, -0x10(r1)
    li r0, 0x0
    li r6, 0x0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    b @L_801AD014
@L_801ACDC4:
    lwz r10, 0x8(r3)
    li r7, 0x0
    b @L_801ACF50
@L_801ACDD0:
    lwz r9, 0x4(r10)
    cmpwi r9, 0x2
    beq @L_801ACDF8
    bge @L_801ACDEC
    cmpwi r9, 0x1
    bge @L_801ACE08
    b @L_801ACF4C
@L_801ACDEC:
    cmpwi r9, 0x4
    bge @L_801ACF4C
    b @L_801ACE00
@L_801ACDF8:
    addi r7, r7, 0x1
    b @L_801ACF4C
@L_801ACE00:
    addi r7, r7, 0x2
    b @L_801ACF4C
@L_801ACE08:
    lwz r9, 0x0(r10)
    cmpwi r9, 0xb
    beq @L_801ACE20
    lwz r9, 0x0(r10)
    cmpwi r9, 0xc
    bne @L_801ACE70
@L_801ACE20:
    lwz r9, 0xc(r10)
    cmpwi r9, 0x3
    beq @L_801ACE58
    bge @L_801ACE48
    cmpwi r9, 0x1
    beq @L_801ACE60
    bge @L_801ACE68
    cmpwi r9, 0x0
    bge @L_801ACE58
    b @L_801ACEA8
@L_801ACE48:
    cmpwi r9, 0x5
    beq @L_801ACE68
    bge @L_801ACEA8
    b @L_801ACE60
@L_801ACE58:
    li r8, 0x2
    b @L_801ACEA8
@L_801ACE60:
    li r8, 0x3
    b @L_801ACEA8
@L_801ACE68:
    li r8, 0x4
    b @L_801ACEA8
@L_801ACE70:
    lwz r9, 0xc(r10)
    cmpwi r9, 0x4
    beq @L_801ACEA4
    bge @L_801AD010
    cmpwi r9, 0x2
    bge @L_801ACE9C
    cmpwi r9, 0x0
    bge @L_801ACE94
    b @L_801AD010
@L_801ACE94:
    li r8, 0x1
    b @L_801ACEA8
@L_801ACE9C:
    li r8, 0x2
    b @L_801ACEA8
@L_801ACEA4:
    li r8, 0x4
@L_801ACEA8:
    lwz r9, 0x0(r10)
    cmpwi r9, 0xa
    beq @L_801ACF10
    bge @L_801ACED0
    cmpwi r9, 0x0
    beq @L_801ACEE4
    blt @L_801AD010
    cmpwi r9, 0x9
    bge @L_801ACEEC
    b @L_801AD010
@L_801ACED0:
    cmpwi r9, 0x15
    bge @L_801AD010
    cmpwi r9, 0xd
    bge @L_801ACF30
    b @L_801ACF28
@L_801ACEE4:
    addi r7, r7, 0x1
    b @L_801ACF4C
@L_801ACEEC:
    lwz r9, 0x8(r10)
    cmpwi r9, 0x0
    bne @L_801ACF04
    rlwinm r9, r8, 1, 0, 30
    add r7, r7, r9
    b @L_801ACF4C
@L_801ACF04:
    mulli r9, r8, 0x3
    add r7, r7, r9
    b @L_801ACF4C
@L_801ACF10:
    lwz r9, 0x8(r10)
    cmpwi r9, 0x0
    bne @L_801ACF4C
    mulli r9, r8, 0x3
    add r7, r7, r9
    b @L_801ACF4C
@L_801ACF28:
    add r7, r7, r8
    b @L_801ACF4C
@L_801ACF30:
    lwz r9, 0x8(r10)
    cmpwi r9, 0x0
    bne @L_801ACF44
    add r7, r7, r8
    b @L_801ACF4C
@L_801ACF44:
    rlwinm r9, r8, 1, 0, 30
    add r7, r7, r9
@L_801ACF4C:
    addi r10, r10, 0x18
@L_801ACF50:
    lwz r9, 0x0(r10)
    cmpwi r9, 0xff
    bne @L_801ACDD0
    lwz r31, 0x10(r3)
    lhz r9, 0xe(r3)
    cmplwi r31, 0x0
    rlwinm r12, r9, 5, 0, 26
    beq @L_801AD010
    li r11, 0x0
    b @L_801AD008
@L_801ACF78:
    lbzx r9, r31, r11
    addi r11, r11, 0x1
    rlwinm. r30, r9, 0, 24, 28
    beq @L_801AD008
    add r9, r31, r11
    lbzx r10, r31, r11
    lbz r9, 0x1(r9)
    cmpwi r30, 0x98
    clrlslwi r10, r10, 16, 8
    add r10, r10, r9
    mullw r9, r10, r7
    add r0, r0, r10
    add r11, r9, r11
    addi r11, r11, 0x2
    beq @L_801ACFF4
    bge @L_801ACFD0
    cmpwi r30, 0x90
    beq @L_801ACFDC
    bge @L_801AD008
    cmpwi r30, 0x80
    beq @L_801AD000
    b @L_801AD008
@L_801ACFD0:
    cmpwi r30, 0xa0
    beq @L_801ACFF4
    b @L_801AD008
@L_801ACFDC:
    lis r9, 0xaaab
    subi r9, r9, 0x5555
    mulhwu r9, r9, r10
    rlwinm r9, r9, 31, 1, 31
    add r6, r6, r9
    b @L_801AD008
@L_801ACFF4:
    add r6, r10, r6
    subi r6, r6, 0x2
    b @L_801AD008
@L_801AD000:
    rlwinm r9, r10, 31, 1, 31
    add r6, r6, r9
@L_801AD008:
    cmpw r11, r12
    blt @L_801ACF78
@L_801AD010:
    lwz r3, 0x4(r3)
@L_801AD014:
    cmplwi r3, 0x0
    bne @L_801ACDC4
    cmplwi r4, 0x0
    beq @L_801AD028
    stw r0, 0x0(r4)
@L_801AD028:
    cmplwi r5, 0x0
    beq @L_801AD034
    stw r6, 0x0(r5)
@L_801AD034:
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    addi r1, r1, 0x10
    blr
