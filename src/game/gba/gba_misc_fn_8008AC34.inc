    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lis r3, -32704
    rlwinm r30, r29, 2, 0, 29
    addi r0, r3, -19688
    add r31, r0, r30
    subi r31, r31, 0x4
    lwz r0, 0x0(r31)
    cmpwi r0, 0x2
    beq @L_8008ADB8
    bge @L_8008AC80
    cmpwi r0, 0x1
    bge @L_8008AC8C
    b @L_8008ADD4
@L_8008AC80:
    cmpwi r0, 0x4
    bge @L_8008ADD4
    b @L_8008AD3C
@L_8008AC8C:
    subi r3, r29, 0x1
    addi r4, r1, 0x8
    bl fn_80073A44
    cmpwi r3, 0x0
    bne @L_8008ACC8
    rlwinm r7, r29, 1, 0, 30
    addi r6, r13, -24988
    addi r0, r13, -24996
    lhz r5, 0x8(r1)
    add r4, r0, r7
    add r6, r6, r7
    li r0, 0x0
    sth r5, -0x2(r4)
    sth r0, -0x2(r6)
    b @L_8008ADEC
@L_8008ACC8:
    cmpwi r3, 0x2
    bgt @L_8008ACFC
    rlwinm r0, r29, 1, 0, 30
    addi r4, r13, -24988
    add r5, r4, r0
    lhz r4, -0x2(r5)
    addi r4, r4, 0x1
    rlwinm r0, r4, 0, 16, 31
    sth r4, -0x2(r5)
    cmplwi r0, 0xa
    bgt @L_8008ACFC
    li r3, 0x0
    b @L_8008ADEC
@L_8008ACFC:
    rlwinm r7, r29, 1, 0, 30
    addi r0, r13, -24996
    add r8, r0, r7
    li r6, 0x0
    subi r8, r8, 0x2
    lis r4, -32704
    sth r6, 0x0(r8)
    addi r4, r4, -19704
    addi r0, r13, -24988
    add r5, r4, r30
    stw r6, 0x0(r31)
    add r4, r0, r7
    stw r6, -0x4(r5)
    sth r6, -0x2(r4)
    sth r6, 0x0(r8)
    b @L_8008ADEC
@L_8008AD3C:
    subi r3, r29, 0x1
    bl fn_80073990
    cmpwi r3, 0x0
    bne @L_8008AD64
    rlwinm r0, r29, 1, 0, 30
    addi r4, r13, -24988
    add r4, r4, r0
    li r0, 0x0
    sth r0, -0x2(r4)
    b @L_8008ADA0
@L_8008AD64:
    cmpwi r3, 0x2
    bgt @L_8008AD98
    rlwinm r0, r29, 1, 0, 30
    addi r4, r13, -24988
    add r5, r4, r0
    lhz r4, -0x2(r5)
    addi r4, r4, 0x1
    rlwinm r0, r4, 0, 16, 31
    sth r4, -0x2(r5)
    cmplwi r0, 0xa
    bgt @L_8008AD98
    li r3, 0x0
    b @L_8008ADA0
@L_8008AD98:
    li r0, 0x0
    stw r0, 0x0(r31)
@L_8008ADA0:
    rlwinm r0, r29, 1, 0, 30
    addi r4, r13, -24996
    add r4, r4, r0
    li r0, 0x0
    sth r0, -0x2(r4)
    b @L_8008ADEC
@L_8008ADB8:
    rlwinm r0, r29, 1, 0, 30
    addi r3, r13, -24996
    add r3, r3, r0
    li r0, 0x0
    sth r0, -0x2(r3)
    li r3, 0x0
    b @L_8008ADEC
@L_8008ADD4:
    rlwinm r0, r29, 1, 0, 30
    addi r3, r13, -24996
    add r3, r3, r0
    li r0, 0x0
    sth r0, -0x2(r3)
    li r3, 0x1
@L_8008ADEC:
    lis r4, -32704
    addi r0, r4, -19704
    add r4, r0, r30
    stw r3, -0x4(r4)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
