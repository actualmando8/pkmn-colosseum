    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r10, r4
    mr r6, r5
    lwz r9, lbl_80478B08(r13)
    li r5, 0x0
    lhz r8, 0x0(r9)
    b @L_800FBED0
@L_800FBEA0:
    clrlwi r0, r5, 24
    lwz r4, 0x20(r9)
    mulli r0, r0, 0x68
    addi r5, r5, 0x1
    add r7, r4, r0
    lbz r0, 0x0(r7)
    cmplwi r0, 0x0
    beq @L_800FBED0
    lwz r0, 0x1c(r7)
    cmplw r0, r3
    bne @L_800FBED0
    b @L_800FBEE0
@L_800FBED0:
    clrlwi r0, r5, 24
    cmpw r0, r8
    blt @L_800FBEA0
    li r7, 0x0
@L_800FBEE0:
    cmplwi r7, 0x0
    bne @L_800FBEF0
    li r3, -0x1
    b @L_800FBF00
@L_800FBEF0:
    lbz r4, 0x44(r7)
    mr r3, r7
    mr r5, r10
    bl fn_800FC7E0
@L_800FBF00:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
