    nofralloc
    lwz r0, lbl_8047A968(r13)
    cmplwi r0, 0x0
    bne @L_800AF174
    stw r3, lbl_8047A96C(r13)
    li r0, 0x0
    stw r3, lbl_8047A964(r13)
    stw r3, lbl_8047A968(r13)
    stw r0, 0x3c(r3)
    stw r0, 0x38(r3)
    blr
    @L_800AF174:
    mr r5, r0
    b @L_800AF1BC
    @L_800AF17C:
    lwz r4, 0x4(r3)
    lwz r0, 0x4(r5)
    cmplw r4, r0
    bge @L_800AF1B8
    lwz r0, 0x3c(r5)
    stw r0, 0x3c(r3)
    stw r3, 0x3c(r5)
    stw r5, 0x38(r3)
    lwz r4, 0x3c(r3)
    cmplwi r4, 0x0
    bne @L_800AF1B0
    stw r3, lbl_8047A968(r13)
    b @L_800AF1C4
    @L_800AF1B0:
    stw r3, 0x38(r4)
    b @L_800AF1C4
    @L_800AF1B8:
    lwz r5, 0x38(r5)
    @L_800AF1BC:
    cmplwi r5, 0x0
    bne @L_800AF17C
    @L_800AF1C4:
    cmplwi r5, 0x0
    bnelr
    lwz r4, lbl_8047A964(r13)
    li r0, 0x0
    stw r3, 0x38(r4)
    stw r0, 0x38(r3)
    lwz r0, lbl_8047A964(r13)
    stw r0, 0x3c(r3)
    stw r3, lbl_8047A964(r13)
    blr
