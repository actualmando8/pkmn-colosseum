    nofralloc
    clrlwi r0, r3, 24
    lwz r4, -21104(r13)
    mulli r3, r0, 0xe0
    addi r7, r3, 0x300
    li r0, 0x4
    add r7, r4, r7
    mtctr r0
    addi r6, r7, 0x4
    li r8, 0x0
    li r9, 0x1
@L_801E6144:
    lwz r3, -21212(r13)
    subi r5, r9, 0x1
    lbzx r0, r3, r5
    cmplwi r0, 0x0
    beq @L_801E6190
    lwz r3, -21204(r13)
    slwi r0, r8, 1
    lhzx r0, r3, r0
    subf r0, r0, r8
    stw r0, 0x8c(r6)
    lwz r3, -21212(r13)
    lwz r4, -21204(r13)
    lbzx r0, r3, r5
    add r8, r8, r0
    slwi r3, r8, 1
    subi r0, r3, 0x2
    lhzx r0, r4, r0
    stw r0, 0x44(r6)
    b @L_801E619C
@L_801E6190:
    li r0, -0x1
    stw r0, 0x44(r6)
    stw r0, 0x8c(r6)
@L_801E619C:
    lwz r3, -21212(r13)
    addi r5, r9, 0x0
    addi r9, r9, 0x1
    lbzx r0, r3, r5
    addi r6, r6, 0x4
    cmplwi r0, 0x0
    beq @L_801E61F0
    lwz r3, -21204(r13)
    slwi r0, r8, 1
    lhzx r0, r3, r0
    subf r0, r0, r8
    stw r0, 0x8c(r6)
    lwz r3, -21212(r13)
    lwz r4, -21204(r13)
    lbzx r0, r3, r5
    add r8, r8, r0
    slwi r3, r8, 1
    subi r0, r3, 0x2
    lhzx r0, r4, r0
    stw r0, 0x44(r6)
    b @L_801E61FC
@L_801E61F0:
    li r0, -0x1
    stw r0, 0x44(r6)
    stw r0, 0x8c(r6)
@L_801E61FC:
    lwz r3, -21212(r13)
    addi r5, r9, 0x0
    addi r9, r9, 0x1
    lbzx r0, r3, r5
    addi r6, r6, 0x4
    cmplwi r0, 0x0
    beq @L_801E6250
    lwz r3, -21204(r13)
    slwi r0, r8, 1
    lhzx r0, r3, r0
    subf r0, r0, r8
    stw r0, 0x8c(r6)
    lwz r3, -21212(r13)
    lwz r4, -21204(r13)
    lbzx r0, r3, r5
    add r8, r8, r0
    slwi r3, r8, 1
    subi r0, r3, 0x2
    lhzx r0, r4, r0
    stw r0, 0x44(r6)
    b @L_801E625C
@L_801E6250:
    li r0, -0x1
    stw r0, 0x44(r6)
    stw r0, 0x8c(r6)
@L_801E625C:
    lwz r3, -21212(r13)
    addi r5, r9, 0x0
    addi r9, r9, 0x1
    lbzx r0, r3, r5
    addi r6, r6, 0x4
    cmplwi r0, 0x0
    beq @L_801E62B0
    lwz r3, -21204(r13)
    slwi r0, r8, 1
    lhzx r0, r3, r0
    subf r0, r0, r8
    stw r0, 0x8c(r6)
    lwz r3, -21212(r13)
    lwz r4, -21204(r13)
    lbzx r0, r3, r5
    add r8, r8, r0
    slwi r3, r8, 1
    subi r0, r3, 0x2
    lhzx r0, r4, r0
    stw r0, 0x44(r6)
    b @L_801E62BC
@L_801E62B0:
    li r0, -0x1
    stw r0, 0x44(r6)
    stw r0, 0x8c(r6)
@L_801E62BC:
    addi r6, r6, 0x4
    addi r9, r9, 0x1
    bdnz @L_801E6144
    lis r3, 0x10
    subi r0, r3, 0x1
    stw r0, 0x88(r7)
    blr
