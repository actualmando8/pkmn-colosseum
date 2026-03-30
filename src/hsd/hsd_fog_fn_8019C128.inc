    nofralloc
    cmplwi r3, 0x0
    bne @L_8019C138
    lis r3, lbl_804653A8@ha
    addi r3, r3, lbl_804653A8@l
    @L_8019C138:
    lis r6, 0x4470
    subi r6, r6, 0x79a9
    mulhwu r6, r6, r4
    subf r0, r6, r4
    srwi r0, r0, 1
    add r0, r0, r6
    srwi r0, r0, 6
    mulli r0, r0, 0x65
    subf r0, r0, r4
    slwi r0, r0, 2
    lwzx r3, r3, r0
    b @L_8019C190
    @L_8019C168:
    lwz r0, 0x4(r3)
    cmplw r0, r4
    bne @L_8019C18C
    cmplwi r5, 0x0
    beq @L_8019C184
    li r0, 0x1
    stw r0, 0x0(r5)
    @L_8019C184:
    lwz r3, 0x8(r3)
    blr
    @L_8019C18C:
    lwz r3, 0x0(r3)
    @L_8019C190:
    cmplwi r3, 0x0
    bne @L_8019C168
    cmplwi r5, 0x0
    beq @L_8019C1A8
    li r0, 0x0
    stw r0, 0x0(r5)
    @L_8019C1A8:
    li r3, 0x0
    blr
